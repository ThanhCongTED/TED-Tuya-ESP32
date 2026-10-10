/**
 * @file weatherGet.ino
 * @brief Ví dụ về dịch vụ thời tiết Tuya IoT
 * @copyright Copyright (c) 2021-2026 ThanhCongTED.
 */

#include "tLed.h"

#include "TuyaIoT.h"
#include "TuyaIoTWeather.h"
#include <Log.h>
#include <Ticker.h>

#include "tal_memory.h"

TuyaIoTWeatherClass TuyaWeather;
Ticker              freeHeapTicker;

#define ledPin LED_BUILTIN
// Bật LED khi mức output thấp
tLed led(ledPin, LOW);

// nút nhấn
#define buttonPin         BUTTON_BUILTIN
#define buttonPressLevel  LOW
#define buttonDebounceMs  (50u)
#define buttonLongPressMs (3 * 1000u)


// ==================== CẤU HÌNH TUYA ====================
const char *pid = "ekdehkpnjp8squlk";
const char *mcu_ver = "2.1.0";

// Thông tin xác thực Tuya
#define THANHCONGTED_UUID    "uuidxxxxxxxxxxxxxxxx"
#define THANHCONGTED_AUTHKEY "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"

#define DPID_SWITCH 1

void tuyaIoTEventCallback(tuya_event_msg_t *event);
void buttonCheck(void);
void weatherGetDemo(void);

void heapCallback()
{
    PR_NOTICE("Free heap: %d", tal_system_get_free_heap_size());
}

void setup()
{
    // đặt code setup của bạn ở đây, chạy một lần:
    Serial.begin(115200);

    Log.begin();

    freeHeapTicker.attach(5, heapCallback);

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

    // "PROJECT_VERSION" đến từ trường "PROJECT_VERSION" trong "appConfig.json"
    TuyaIoT.begin(pid, mcu_ver);
}

void loop()
{
    // đặt code chính của bạn ở đây, chạy lặp đi lặp lại:
    led.update();

    weatherGetDemo();

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

void weatherGetDemo(void)
{
    int            rt           = OPRT_OK;
    static uint8_t isGetWeather = 0;

    if (TuyaIoT.isTimeSync() && (0 == isGetWeather)) {
        int weather = 0, temp = 0, humi = 0, realFeel = 0, mbar = 0, uvi = 0;
        rt = TuyaWeather.getCurrentConditions(weather, temp, humi, realFeel, mbar, uvi);
        if (OPRT_OK != rt) {
            return;
        }
        Serial.print("weather: ");
        Serial.println(weather);
        Serial.print("temp: ");
        Serial.println(temp);
        Serial.print("humi: ");
        Serial.println(humi);
        Serial.print("realFeel: ");
        Serial.println(realFeel);
        Serial.print("mbar: ");
        Serial.println(mbar);
        Serial.print("uvi: ");
        Serial.println(uvi);

        int todayHigh = 0, todayLow = 0;
        rt = TuyaWeather.getTodayHighLowTemp(todayHigh, todayLow);
        if (OPRT_OK != rt) {
            return;
        }
        Serial.print("todayHigh: ");
        Serial.println(todayHigh);
        Serial.print("todayLow: ");
        Serial.println(todayLow);

        String windDir, windSpeed;
        rt = TuyaWeather.getCurrentWind(windDir, windSpeed);
        if (OPRT_OK != rt) {
            return;
        }
        Serial.print("windDir: ");
        Serial.println(windDir);
        Serial.print("windSpeed: ");
        Serial.println(windSpeed);

        int windLevel = 0;
        rt = TuyaWeather.getCurrentWindCN(windDir, windSpeed, windLevel); // windLevel chỉ hỗ trợ Trung Quốc đại lục
        if (OPRT_OK != rt) {
            return;
        }
        Serial.print("windDir: ");
        Serial.println(windDir);
        Serial.print("windSpeed: ");
        Serial.println(windSpeed);
        Serial.print("windLevel: ");
        Serial.println(windLevel);

        String sunrise, sunset;
        rt = TuyaWeather.getCurrentSunriseSunsetGMT(sunrise, sunset);
        if (OPRT_OK != rt) {
            return;
        }
        Serial.print("sunrise: ");
        Serial.println(sunrise);
        Serial.print("sunset: ");
        Serial.println(sunset);

        rt = TuyaWeather.getCurrentSunriseSunsetLocal(sunrise, sunset);
        if (OPRT_OK != rt) {
            return;
        }
        Serial.print("sunrise: ");
        Serial.println(sunrise);
        Serial.print("sunset: ");
        Serial.println(sunset);

        int aqi = 0, qualityLevel = 0, pm25 = 0, pm10 = 0, o3 = 0, no2 = 0, co = 0, so2 = 0;
        rt = TuyaWeather.getCurrentAQI(aqi, qualityLevel, pm25, pm10, o3, no2, co, so2);
        if (OPRT_OK != rt) {
            return;
        }
        Serial.print("aqi: ");
        Serial.println(aqi);
        Serial.print("qualityLevel: ");
        Serial.println(qualityLevel);
        Serial.print("pm25: ");
        Serial.println(pm25);
        Serial.print("pm10: ");
        Serial.println(pm10);
        Serial.print("o3: ");
        Serial.println(o3);
        Serial.print("no2: ");
        Serial.println(no2);
        Serial.print("co: ");
        Serial.println(co);
        Serial.print("so2: ");
        Serial.println(so2);

        String rank;
        rt = TuyaWeather.getCurrentAQICN(aqi, rank, qualityLevel, pm25, pm10, o3, no2, co,
                                         so2); // rank chỉ hỗ trợ Trung Quốc đại lục
        if (OPRT_OK != rt) {
            return;
        }
        Serial.print("aqi: ");
        Serial.println(aqi);
        Serial.print("rank: ");
        Serial.println(rank);
        Serial.print("qualityLevel: ");
        Serial.println(qualityLevel);
        Serial.print("pm25: ");
        Serial.println(pm25);
        Serial.print("pm10: ");
        Serial.println(pm10);
        Serial.print("o3: ");
        Serial.println(o3);
        Serial.print("no2: ");
        Serial.println(no2);
        Serial.print("co: ");
        Serial.println(co);
        Serial.print("so2: ");
        Serial.println(so2);

        int              number = 7;
        std::vector<int> weatherV, tempV, humiV, uviV, mbarV;
        // temp, pressure không hỗ trợ dự báo ở Trung Quốc đại lục
        // vui lòng sử dụng getForecastConditionsCN() để lấy dự báo thời tiết ở Trung Quốc đại lục
        rt = TuyaWeather.getForecastConditions(number, weatherV, tempV, humiV, uviV, mbarV);
        if (OPRT_OK != rt) {
            return;
        }
        for (int i = 0; i < number; i++) {
            Serial.print("weather[");
            Serial.print(i);
            Serial.print("]: ");
            Serial.println(weatherV[i]);
            Serial.print("temp[");
            Serial.print(i);
            Serial.print("]: ");
            Serial.println(tempV[i]);
            Serial.print("humi[");
            Serial.print(i);
            Serial.print("]: ");
            Serial.println(humiV[i]);
            Serial.print("uvi[");
            Serial.print(i);
            Serial.print("]: ");
            Serial.println(uviV[i]);
            Serial.print("mbarV[");
            Serial.print(i);
            Serial.print("]: ");
            Serial.println(mbarV[i]);
        }

        number = 7;
        rt     = TuyaWeather.getForecastConditionsCN(number, weatherV, humiV, uviV);
        if (OPRT_OK != rt) {
            return;
        }
        for (int i = 0; i < number; i++) {
            Serial.print("weather[");
            Serial.print(i);
            Serial.print("]: ");
            Serial.println(weatherV[i]);
            Serial.print("humi[");
            Serial.print(i);
            Serial.print("]: ");
            Serial.println(humiV[i]);
            Serial.print("uvi[");
            Serial.print(i);
            Serial.print("]: ");
            Serial.println(uviV[i]);
        }

        std::vector<String> windDirV, windSpeedV;
        number = 7;
        rt     = TuyaWeather.getForecastWind(number, windDirV, windSpeedV);
        if (OPRT_OK != rt) {
            return;
        }
        for (int i = 0; i < number; i++) {
            Serial.print("windDir[");
            Serial.print(i);
            Serial.print("]: ");
            Serial.println(windDirV[i]);
            Serial.print("windSpeed[");
            Serial.print(i);
            Serial.print("]: ");
            Serial.println(windSpeedV[i]);
        }

        std::vector<int> highTempV, lowTempV;
        number = 7;
        rt     = TuyaWeather.getForecastHighLowTemp(number, highTempV, lowTempV);
        if (OPRT_OK != rt) {
            return;
        }
        for (int i = 0; i < number; i++) {
            Serial.print("highTemp[");
            Serial.print(i);
            Serial.print("]: ");
            Serial.println(highTempV[i]);
            Serial.print("lowTemp[");
            Serial.print(i);
            Serial.print("]: ");
            Serial.println(lowTempV[i]);
        }

        String province, city, area;
        rt = TuyaWeather.getCity(province, city, area);
        if (OPRT_OK != rt) {
            return;
        }
        Serial.print("province: ");
        Serial.println(province);
        Serial.print("city: ");
        Serial.println(city);
        Serial.print("area: ");
        Serial.println(area);

        isGetWeather = 1;
    }
}