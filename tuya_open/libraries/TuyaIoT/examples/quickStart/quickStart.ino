/**
 * @file quickStart.ino thanhcongted.com
 * @brief Ví dụ bắt đầu nhanh với Tuya IoT
 * @copyright Copyright (c) 2021-2026 ThanhCongTED.
 */

#include "tLed.h"
#include "TuyaIoT.h"
#include <Log.h>

#define ledPin LED_BUILTIN
// Bật LED khi mức output thấp
tLed led(ledPin, LOW);

// nút nhấn
#define buttonPin         BUTTON_BUILTIN
#define buttonPressLevel  LOW
#define buttonDebounceMs  (50u)
#define buttonLongPressMs (3 * 1000u)

// ==================== CẤU HÌNH TUYA ====================
const char *pid = "2avicuxv6zgeiquf";
const char *mcu_ver = "2.1.0";

// Thông tin xác thực Tuya
// https://thanhcongted.com/webinstaller/tuya_pay.html

#define THANHCONGTED_UUID    "uuidxxxxxxxxxxxxxxxx"
#define THANHCONGTED_AUTHKEY "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
#define DPID_SWITCH 1

void tuyaIoTEventCallback(tuya_event_msg_t *event);
void buttonCheck(void);

void setup()
{
    // đặt code setup của bạn ở đây, chạy một lần:
    Serial.begin(115200);

    Log.begin();

    // led
    led.off();

    // khởi tạo nút nhấn
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
    Serial.print("uuid: ");
    Serial.println(license.uuid);
    Serial.print("authkey: ");
    Serial.println(license.authkey);
    TuyaIoT.setLicense(license.uuid, license.authkey);
    TuyaIoT.begin(pid, mcu_ver);
}

void loop()
{
    // đặt code chính của bạn ở đây, chạy lặp đi lặp lại:
    led.update();

    // kiểm tra nhấn nút
    buttonCheck();

    delay(10);
}

void tuyaIoTEventCallback(tuya_event_msg_t *event)
{
    int ledState = 0;

    tuya_event_id_t event_id = TuyaIoT.eventGetId(event);

    switch (event_id) {
    case TUYA_EVENT_BIND_START: {
        led.blink(500);
    } break;
    case TUYA_EVENT_ACTIVATE_SUCCESSED: {
        led.off();
    } break;
    case TUYA_EVENT_MQTT_CONNECTED: {
        // Cập nhật tất cả DP
        Serial.println("---> TUYA_EVENT_MQTT_CONNECTED");
        uint8_t curState = led.getState();
        TuyaIoT.write(DPID_SWITCH, curState);
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
                TuyaIoT.read(event, DPID_SWITCH, ledState);
                Serial.print("Nhận DPID_SWITCH: ");
                Serial.println(ledState);
                led.setState(ledState);
                TuyaIoT.write(DPID_SWITCH, ledState);
            } break;
            default:
                break;
            }
        }
    } break;
    default:
        break;
    }
}

void buttonClick()
{
    Serial.println("Đã nhấn nút");
    uint8_t ledState = led.getState();

    ledState = !ledState;
    led.setState(ledState);

    Serial.print("Gửi DPID_SWITCH: ");
    Serial.println(ledState);
    TuyaIoT.write(DPID_SWITCH, ledState);
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