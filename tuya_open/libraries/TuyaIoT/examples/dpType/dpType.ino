/**
 * @file dpType.ino
 * @brief Ví dụ về các loại DP của Tuya IoT
 * @copyright Copyright (c) 2021-2026 ThanhCongTED.
 */

#include "TuyaIoT.h"
#include "Log.h"

// nút nhấn
#define buttonPin         BUTTON_BUILTIN
#define buttonPressLevel  LOW
#define buttonDebounceMs  (50u)
#define buttonLongPressMs (3 * 1000u)

#define DPID_SWITCH 20
#define DPID_MODE   21
#define DPID_BRIGHT 22
#define DPID_BITMAP 101
#define DPID_STRING 102
#define DPID_RAW    103


// ==================== CẤU HÌNH TUYA ====================
const char *pid = "2avicuxv6zgeiquf";
const char *mcu_ver = "2.1.0";

// Thông tin xác thực Tuya
#define THANHCONGTED_UUID    "uuidxxxxxxxxxxxxxxxx"
#define THANHCONGTED_AUTHKEY "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"

void tuyaIoTEventCallback(tuya_event_msg_t *event);
void buttonCheck(void);

void setup()
{
    // đặt code setup của bạn ở đây, chạy một lần:
    Serial.begin(115200);

    Log.begin();

    // nút nhấn
    pinMode(buttonPin, INPUT_PULLUP);

    TuyaIoT.setEventCallback(tuyaIoTEventCallback);

    // license
    tuya_iot_license_t license;
    int                rt = TuyaIoT.readBoardLicense(&license);
    if (OPRT_OK != rt) {
        license.uuid    = (char *)THANHCONGTED_UUID;
        license.authkey = (char *)THANHCONGTED_AUTHKEY;
        Serial.println("Thay thế nội dung THANHCONGTED_UUID và THANHCONGTED_AUTHKEY, nếu không demo sẽ không hoạt động");
    }
    Serial.print("THANHCONGTED_UUID: ");
    Serial.println(license.uuid);
    Serial.print("THANHCONGTED_AUTHKEY: ");
    Serial.println(license.authkey);
    TuyaIoT.setLicense(license.uuid, license.authkey);
    TuyaIoT.begin(pid, mcu_ver);
}

void loop()
{
    // đặt code chính của bạn ở đây, chạy lặp đi lặp lại:

    // kiểm tra nhấn nút
    buttonCheck();

    delay(10);
}

void tuyaIoTEventCallback(tuya_event_msg_t *event)
{
    tuya_event_id_t event_id = TuyaIoT.eventGetId(event);

    switch (event_id) {
    case TUYA_EVENT_BIND_START: {
        PR_DEBUG("TUYA_EVENT_BIND_START");
    } break;
    case TUYA_EVENT_ACTIVATE_SUCCESSED: {
        PR_DEBUG("TUYA_EVENT_ACTIVATE_SUCCESSED");
    } break;
    case TUYA_EVENT_TIMESTAMP_SYNC: {
        tal_time_set_posix(event->value.asInteger, 1);
    } break;
    case TUYA_EVENT_DP_RECEIVE_OBJ: {
        uint16_t dpNum = TuyaIoT.eventGetDpNum(event);
        for (uint16_t i = 0; i < dpNum; i++) {
            uint8_t dpid = TuyaIoT.eventGetDpId(event, i);
            switch (dpid) {
            case DPID_SWITCH: {
                bool switchStatus = 0;
                TuyaIoT.read(event, DPID_SWITCH, switchStatus);
                TuyaIoT.write(DPID_SWITCH, switchStatus);
                PR_DEBUG("switchStatus: %d", switchStatus);
            } break;
            case DPID_MODE: {
                uint32_t mode = 0;
                TuyaIoT.read(event, DPID_MODE, mode);
                TuyaIoT.write(DPID_MODE, mode);
                PR_DEBUG("mode: %d", mode);
            } break;
            case DPID_BRIGHT: {
                int brightValue = 0;
                TuyaIoT.read(event, DPID_BRIGHT, brightValue);
                TuyaIoT.write(DPID_BRIGHT, brightValue);
                PR_DEBUG("brightValue: %d", brightValue);
            } break;
            case DPID_STRING: {
                char *strValue = NULL;
                TuyaIoT.read(event, DPID_STRING, strValue);
                TuyaIoT.write(DPID_STRING, strValue);
            } break;
            default:
                break;
            }
        }
    } break;
    case TUYA_EVENT_DP_RECEIVE_RAW: {
        uint8_t *rawValue = NULL;
        uint16_t len      = 0;
        TuyaIoT.read(event, DPID_RAW, rawValue, len);
        PR_DEBUG("---> len:%d", len);
        PR_HEXDUMP_DEBUG("raw", rawValue, len);
        TuyaIoT.write(DPID_RAW, rawValue, len);
    } break;
    default:
        break;
    }
}

void buttonClick()
{
    Serial.println("Đã nhấn nút");
}

void buttonLongPressStart()
{
    Serial.println("Nhấn giữ nút, xóa thiết bị Tuya IoT.");
    TuyaIoT.remove();
}

void buttonCheck(void)
{
    static uint32_t buttonPressMs = 0;
    static uint8_t  isPress       = 0;

    if (digitalRead(buttonPin) == buttonPressLevel) {
        if (isPress == 0) {
            buttonPressMs = millis();
            isPress       = 1;
        }

        // chống dội nút nhấn
        if ((1 == isPress) && ((millis() - buttonPressMs) > buttonDebounceMs)) {
            isPress = 2;
        }

        // kiểm tra nhấn giữ
        if ((2 == isPress) && ((millis() - buttonPressMs) >= buttonLongPressMs)) {
            isPress = 3;
            buttonLongPressStart();
        }
    } else {
        if (isPress == 2) {
            if ((millis() - buttonPressMs) < buttonLongPressMs) {
                buttonClick();
            } else {
                buttonLongPressStart();
            }
        }
        isPress = 0;
    }
}