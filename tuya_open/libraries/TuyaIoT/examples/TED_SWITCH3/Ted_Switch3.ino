/**
 * @file SWITCH3.ino
 * @brief Lập trình giao tiếp với platform Tuya
 * @copyright Bản quyền (c) 2021-2026 ThanhCongTED.
 */

#include "tLed.h"
#include "TuyaIoT.h"
#include <Log.h>

// ==================== CẤU HÌNH LED ====================
// LED 1 - LED built-in

#define ledPin1 2
// Tự động phát hiện LED active LOW trên ESP32
#ifdef ESP32
  #define LED_ON_LEVEL  LOW
#else
  #define LED_ON_LEVEL  HIGH
#endif

tLed led1(ledPin1, LED_ON_LEVEL);

// LED 2 - Bỏ comment nếu có LED vật lý
// #define ledPin2 D1
// tLed led2(ledPin2, LOW);

// LED 3 - Bỏ comment nếu có LED vật lý
// #define ledPin3 D2
// tLed led3(ledPin3, LOW);

// ==================== CẤU HÌNH NÚT NHẤN ====================
#define buttonPin         0
#define buttonPressLevel  LOW
#define buttonDebounceMs  (1000u)
#define buttonLongPressMs (3 * 1000u)

// ==================== CẤU HÌNH TUYA ====================
const char *pid = "ekdehkpnjp8squlk";
const char *mcu_ver = "2.1.0";

// Thông tin xác thực Tuya
#define THANHCONGTED_UUID    "uuidxxxxxxxxxxxxxxxx"
#define THANHCONGTED_AUTHKEY "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"

// ==================== DP ID DEFINITIONS ====================
#define DPID_SWITCH_1     1
#define DPID_SWITCH_2     2
#define DPID_SWITCH_3     3
#define DPID_COUNTDOWN_1  7
#define DPID_COUNTDOWN_2  8
#define DPID_COUNTDOWN_3  9

#define NUM_SWITCHES      3

// ==================== KHAI BÁO HÀM ====================
void tuyaIoTEventCallback(tuya_event_msg_t *event);
void buttonCheck(void);
void updateAllLeds(void);
void sendAllDpStates(void);
uint8_t getChannelState(int index);
void setChannelState(int index, uint8_t state);

// ==================== QUẢN LÝ LED ====================
// Mảng con trỏ LED - nullptr cho kênh không có LED vật lý
tLed* leds[NUM_SWITCHES] = { &led1, nullptr, nullptr };
// Nếu có đủ 3 LED, dùng:
// tLed* leds[NUM_SWITCHES] = { &led1, &led2, &led3 };

// Mảng DP ID switch tương ứng
const uint8_t dpidSwitchArray[NUM_SWITCHES] = {
    DPID_SWITCH_1,
    DPID_SWITCH_2,
    DPID_SWITCH_3
};

// Mảng DP ID countdown tương ứng
const uint8_t dpidCountdownArray[NUM_SWITCHES] = {
    DPID_COUNTDOWN_1,
    DPID_COUNTDOWN_2,
    DPID_COUNTDOWN_3
};

// Mảng lưu trạng thái ảo cho kênh không có LED vật lý
uint8_t virtualStates[NUM_SWITCHES] = { 0, 0, 0 };

// Mảng lưu thời gian countdown (giây) cho mỗi kênh
uint32_t countdownSeconds[NUM_SWITCHES] = { 0, 0, 0 };
// Mảng lưu thời điểm bắt đầu countdown (millis)
uint32_t countdownStartMs[NUM_SWITCHES] = { 0, 0, 0 };
// Mảng lưu trạng thái countdown có đang chạy không
bool     countdownActive[NUM_SWITCHES] = { false, false, false };

// ==================== SETUP ====================
void setup()
{
    Serial.begin(115200);
    //Log.begin();

    Serial.println("\n=== Khởi động Tuya IoT 3 kênh Switch ===");

    // Khởi tạo tất cả LED
    for (int i = 0; i < NUM_SWITCHES; i++) {
        if (leds[i] != nullptr) {
            leds[i]->off();
        }
    }

    // Khởi tạo nút nhấn
    pinMode(buttonPin, INPUT_PULLUP);

    TuyaIoT.setEventCallback(tuyaIoTEventCallback);

    // Đọc license từ board
    tuya_iot_license_t license;
    int rt = TuyaIoT.readBoardLicense(&license);
    if (OPRT_OK != rt) {
        license.uuid    = (char *)THANHCONGTED_UUID;
        license.authkey = (char *)THANHCONGTED_AUTHKEY;
        Serial.println("Hãy thay thế THANHCONGTED_UUID và THANHCONGTED_AUTHKEY");
    }

    Serial.print("uuid: ");
    Serial.println(license.uuid);
    Serial.print("authkey: ");
    Serial.println(license.authkey);

    TuyaIoT.setLicense(license.uuid, license.authkey);

    TuyaIoT.begin(pid, mcu_ver);
}

// ==================== LOOP ====================
void loop()
{
    // Cập nhật trạng thái LED (blink)
    updateAllLeds();

    // Xử lý countdown
    processCountdown();

    // Kiểm tra nút nhấn
    buttonCheck();

    delay(10);
}

// ==================== HÀM HỖ TRỢ ====================

/**
 * @brief Lấy trạng thái hiện tại của một kênh
 */
uint8_t getChannelState(int index)
{
    if (index < 0 || index >= NUM_SWITCHES) return 0;

    if (leds[index] != nullptr) {
        return leds[index]->getState();
    }
    return virtualStates[index];
}

/**
 * @brief Đặt trạng thái cho một kênh
 */
void setChannelState(int index, uint8_t state)
{
    if (index < 0 || index >= NUM_SWITCHES) return;

    if (leds[index] != nullptr) {
        leds[index]->setState(state);
    } else {
        virtualStates[index] = state;
    }

    // Nếu tắt switch thì hủy countdown
    if (state == 0) {
        countdownActive[index] = false;
        countdownSeconds[index] = 0;
    }
}

/**
 * @brief Cập nhật tất cả LED (xử lý blink)
 */
void updateAllLeds(void)
{
    for (int i = 0; i < NUM_SWITCHES; i++) {
        if (leds[i] != nullptr) {
            leds[i]->update();
        }
    }
}

/**
 * @brief Gửi trạng thái tất cả DP lên Tuya
 */
void sendAllDpStates(void)
{
    for (int i = 0; i < NUM_SWITCHES; i++) {
        uint8_t state = getChannelState(i);
        TuyaIoT.write(dpidSwitchArray[i], state);

        Serial.print("Gửi DPID ");
        Serial.print(dpidSwitchArray[i]);
        Serial.print(": ");
        Serial.println(state);
    }
}

/**
 * @brief Xử lý countdown cho tất cả các kênh
 * Giảm thời gian countdown mỗi giây, khi hết thì tắt switch
 */
void processCountdown(void)
{
    for (int i = 0; i < NUM_SWITCHES; i++) {
        if (!countdownActive[i]) continue;

        uint32_t elapsedSec = (millis() - countdownStartMs[i]) / 1000;

        if (elapsedSec >= countdownSeconds[i]) {
            // Hết thời gian countdown -> tắt switch
            Serial.print("Countdown kênh ");
            Serial.print(i + 1);
            Serial.println(" kết thúc -> TẮT switch");

            countdownActive[i] = false;
            countdownSeconds[i] = 0;

            // Tắt switch
            setChannelState(i, 0);

            // Gửi cập nhật lên Tuya
            TuyaIoT.write(dpidSwitchArray[i], 0);
            TuyaIoT.write(dpidCountdownArray[i], 0);
        }
    }
}

// ==================== EVENT CALLBACK ====================
void tuyaIoTEventCallback(tuya_event_msg_t *event)
{
    tuya_event_id_t event_id = TuyaIoT.eventGetId(event);

    switch (event_id) {

    case TUYA_EVENT_BIND_START: {
        Serial.println("---> Thiết bị vào trạng thái cấu hình WiFi");
        // LED 1 nhấp nháy báo hiệu đang pairing
        if (leds[0] != nullptr) {
            leds[0]->blink(500);
        }
    } break;

    case TUYA_EVENT_ACTIVATE_SUCCESSED: {
        Serial.println("---> Đã kết nối thành công với Tuya");
        for (int i = 0; i < NUM_SWITCHES; i++) {
            if (leds[i] != nullptr) {
                leds[i]->off();
            }
        }
    } break;

    case TUYA_EVENT_MQTT_CONNECTED: {
        Serial.println("---> Đã kết nối vào MQTT của Tuya");
        // Gửi trạng thái tất cả switch lên Tuya
        sendAllDpStates();
    } break;

    case TUYA_EVENT_TIMESTAMP_SYNC: {
        tal_time_set_posix(event->value.asInteger, 1);
    } break;

    case TUYA_EVENT_DP_RECEIVE_OBJ: {
        uint16_t dpNum = TuyaIoT.eventGetDpNum(event);

        for (uint16_t i = 0; i < dpNum; i++) {

            uint8_t dpid = TuyaIoT.eventGetDpId(event, i);

            // ===== XỬ LÝ SWITCH DP =====
            bool isSwitchDp = false;
            int  switchIndex = -1;
            for (int j = 0; j < NUM_SWITCHES; j++) {
                if (dpidSwitchArray[j] == dpid) {
                    isSwitchDp = true;
                    switchIndex = j;
                    break;
                }
            }

            if (isSwitchDp && switchIndex >= 0) {
                uint8_t ledState = 0;
                TuyaIoT.read(event, dpid, ledState);

                Serial.print("Nhận DPID_SWITCH ");
                Serial.print(dpid);
                Serial.print(": ");
                Serial.println(ledState);

                setChannelState(switchIndex, ledState);
                TuyaIoT.write(dpid, ledState);
                continue;
            }

            // ===== XỬ LÝ COUNTDOWN DP =====
            bool isCountdownDp = false;
            int  countdownIndex = -1;
            for (int j = 0; j < NUM_SWITCHES; j++) {
                if (dpidCountdownArray[j] == dpid) {
                    isCountdownDp = true;
                    countdownIndex = j;
                    break;
                }
            }

            if (isCountdownDp && countdownIndex >= 0) {
                uint32_t cdValue = 0;
                TuyaIoT.read(event, dpid, cdValue);

                Serial.print("Nhận DPID_COUNTDOWN ");
                Serial.print(dpid);
                Serial.print(": ");
                Serial.println(cdValue);

                if (cdValue > 0) {
                    // Bắt đầu countdown
                    countdownSeconds[countdownIndex] = cdValue;
                    countdownStartMs[countdownIndex] = millis();
                    countdownActive[countdownIndex] = true;

                    // Bật switch tương ứng
                    setChannelState(countdownIndex, 1);
                    TuyaIoT.write(dpidSwitchArray[countdownIndex], 1);

                    Serial.print("Bắt đầu countdown kênh ");
                    Serial.print(countdownIndex + 1);
                    Serial.print(" trong ");
                    Serial.print(cdValue);
                    Serial.println(" giây");
                } else {
                    // Hủy countdown
                    countdownActive[countdownIndex] = false;
                    countdownSeconds[countdownIndex] = 0;

                    Serial.print("Hủy countdown kênh ");
                    Serial.println(countdownIndex + 1);
                }

                // Gửi phản hồi
                TuyaIoT.write(dpid, cdValue);
                continue;
            }
        }
    } break;

    default:
        break;
    }
}

// ==================== XỬ LÝ NÚT NHẤN ====================
/**
 * @brief Nhấn ngắn: đảo trạng thái Switch 1
 */
void buttonClickLED1()
{
    Serial.println("Nút được nhấn - Đảo trạng thái Switch 1");

    // Chỉ xử lý switch index 0 (DPID_SWITCH_1)
    const int index = 0;

    uint8_t ledState = getChannelState(index);
    ledState = !ledState;
    setChannelState(index, ledState);

    Serial.print("Gửi DPID ");
    Serial.print(dpidSwitchArray[index]);
    Serial.print(": ");
    Serial.println(ledState);

    TuyaIoT.write(dpidSwitchArray[index], ledState);
}
/**
 * @brief Nhấn ngắn: đảo trạng thái tất cả switch
 */
void buttonClick()
{
    Serial.println("Nút được nhấn - Đảo trạng thái tất cả switch");

    for (int i = 0; i < NUM_SWITCHES; i++) {
        uint8_t ledState = getChannelState(i);
        ledState = !ledState;
        setChannelState(i, ledState);

        Serial.print("Gửi DPID ");
        Serial.print(dpidSwitchArray[i]);
        Serial.print(": ");
        Serial.println(ledState);

        TuyaIoT.write(dpidSwitchArray[i], ledState);
    }
}



/**
 * @brief Nhấn giữ: xóa thiết bị khỏi Tuya
 */
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
            Serial.println("Nhấn nút 1 nhấn");
        }

        if ((1 == isPress) && ((millis() - buttonPressMs) > buttonDebounceMs)) {
            isPress = 2;
            Serial.println("Nhấn nút 1 giữ 1000u"); // chống nhiễu
        }

        if ((2 == isPress) && ((millis() - buttonPressMs) >= buttonLongPressMs)) {
            isPress = 3;
            Serial.println("Nhấn nút 1 giữ quá 3 giây");
            buttonLongPressStart();
            
        }

    } else {

        if (isPress == 2) {
            if ((millis() - buttonPressMs) < buttonLongPressMs) {
                Serial.println("Nhấn nút 1 giữ và đã nhả");
                buttonClick();
                
            } else {
                buttonLongPressStart();
                Serial.println("Nhấn nút: 5");
            }
        } 
        if (isPress == 1){
            Serial.println("Nhấn nút 1 đã nhả");
            buttonClickLED1();
        }

        
        isPress = 0;
    }
}