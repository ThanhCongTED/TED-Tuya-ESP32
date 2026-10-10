# 💻 THANHCONGTED – Code ESP32 + Tuya Platform

> **Mô tả:** Code dùng ESP32 + Tuya Platform. Chỉ giữ: Button + Relay + LED WiFi. Xử lý 6 DP: 3 Switch (Bool) + 3 Timer (Value 0–86400s). BUTTON_SW1 giữ 10s → Reset WiFi Tuya (SMART_CONFIG).

---

## 📌 Sơ đồ chân (Pinout) – TED168-V2

| Chân MCU | Định nghĩa | Chức năng |
|----------|------------|-----------|
| D5  | `RELAY_1`  | Relay cho Switch 1 |
| D6  | `RELAY_2`  | Relay cho Switch 2 |
| D7  | `RELAY_3`  | Relay cho Switch 3 |
| D10 | `LED_WIFI` | LED báo trạng thái WiFi |
| D12 | `BUTTON_1` | Nút nhấn Switch 1 (kiêm nút reset WiFi) |
| D14 | `BUTTON_2` | Nút nhấn Switch 2 (nút LEARN) |
| D11 | `BUTTON_3` | Nút nhấn Switch 3 (SENSOR_CUA) |
| D8  | UART RX    | SoftwareSerial RX – Kết nối module Tuya |
| D9  | UART TX    | SoftwareSerial TX – Kết nối module Tuya |

---

## 📊 Data Point (DP) Tuya

| DP ID | Định nghĩa | Kiểu dữ liệu | Mô tả |
|-------|------------|--------------|-------|
| 1 | `DPID_SWITCH_1`    | BOOL  | Trạng thái Switch 1 (0/1) |
| 2 | `DPID_SWITCH_2`    | BOOL  | Trạng thái Switch 2 (0/1) |
| 3 | `DPID_SWITCH_3`    | BOOL  | Trạng thái Switch 3 (0/1) |
| 7 | `DPID_COUNTDOWN_1` | VALUE | Timer đếm ngược Switch 1 (0–86400s) |
| 8 | `DPID_COUNTDOWN_2` | VALUE | Timer đếm ngược Switch 2 (0–86400s) |
| 9 | `DPID_COUNTDOWN_3` | VALUE | Timer đếm ngược Switch 3 (0–86400s) |

---

## ⚙️ Thông số kỹ thuật Firmware

- **Vi điều khiển:** ESP32 
- **Product ID (PID):** `ekdehkpnjp8squlk`
- **Phiên bản MCU:** `2.1.0`
- **UART Tuya:** SoftwareSerial @ 9600 bps (RX=D8, TX=D9)
- **UART Debug:** Hardware Serial @ 115200 bps
- **Thời gian nhấn giữ Reset WiFi:** 10 giây (BUTTON_SW1)
- **Thời gian debounce nút nhấn:** 25 ms
- **Chu kỳ vòng lặp chính (loop):** 10 ms

---

## 🔄 Logic hoạt động Firmware

### 1. Điều khiển Relay
- Mỗi Switch có một Relay tương ứng, điều khiển qua DP Bool từ Tuya hoặc nút nhấn vật lý.
- Relay 1 & 3: **Active LOW** (mức thấp = BẬT)
- Relay 2: **Active HIGH** (mức cao = BẬT)

### 2. Timer đếm ngược
- Mỗi Switch có một Timer riêng (0–86400 giây).
- Khi Timer đếm về 0 → tự động tắt Relay tương ứng.
- Giá trị Timer được cập nhật lên Cloud mỗi giây.

### 3. LED WiFi
- **Đã kết nối Cloud:** LED tắt.
- **Chưa kết nối Cloud:** LED nháy 500ms/lần.

### 4. Reset WiFi
- Nhấn giữ **BUTTON_SW1 trong 10 giây** → kích hoạt chế độ `SMART_CONFIG` để cấu hình lại WiFi Tuya.
- Sau khi nhả nút → thoát chế độ reset.

### 5. Nút nhấn vật lý
- **BUTTON_SW1:** Nhấn ngắn → đảo trạng thái Relay 1. Nhấn giữ 10s → Reset WiFi.
- **BUTTON_SW2:** Nhấn ngắn → đảo trạng thái Relay 2 (nút LEARN).
- **BUTTON_SW3:** Nhấn ngắn → đảo trạng thái Relay 3 (SENSOR_CUA).

---

## 🧩 Cấu trúc Code

```text
├── Khai báo chân (Pin Definitions)
├── Khởi tạo Buttons (JC_Button)
├── Định nghĩa DP Tuya (dp_array)
├── Biến trạng thái (sw_state, timer)
├── Hàm điều khiển Relay (set_relay_1/2/3)
├── Hàm xử lý DP tải xuống (dp_process)
├── Hàm đẩy toàn bộ DP lên Cloud (dp_update_all)
├── Hàm đếm ngược Timer (timer_tick)
├── Hàm cập nhật LED WiFi (wifi_led_update)
├── Setup()
└── Loop()
```

## 📡 Giao thức Tuya

- **Module WiFi:** Tuya TED WiFi (kết nối qua UART)
- **Chế độ cấu hình:** `SMART_CONFIG`
- **Trạng thái WiFi:** `WIFI_CONN_CLOUD` (đã kết nối Cloud)
- **Cập nhật DP:** Sử dụng `mcu_dp_update()` với độ dài tương ứng:
  - Bool: 1 byte
  - Value: 4 byte

---

## ⚠️ Lưu ý khi nạp Firmware

1. Để ESP32 vào chế độ Bootloader: Giữ nút <b>FLASH</b> → nhấn <b>RESET</b> → thả RESET → thả FLASH. Chú ý:Tháo thẻ nhớ SDCard.</li>
2. Sau khi flash thành công, có thể cần RESET thủ công lần đầu.</li>
3. Kiểm tra logic Relay trước khi kết nối tải thực tế (Active LOW/HIGH).
4. **PID Tuya** phải khớp với sản phẩm đã đăng ký trên Tuya IoT Platform.
5. **Phiên bản MCU** (`2.1.0`) phải khớp với cấu hình trên Cloud Tuya.

---

## 🐛 Debug qua Serial (115200 bps)

| Log | Ý nghĩa |
|-----|---------|
| `THANHCONGTED - Switch + Timer`   | Khởi động thành công |
| `✅ Đẩy data lên điện thoại`       | Đồng bộ toàn bộ DP lên Cloud |
| `✅ WiFi đã kết nối Cloud`         | Module Tuya đã kết nối thành công |
| `⏳ Đang cấu hình WiFi...`         | Module chưa kết nối Cloud |
| `🔁 Nhấn giữ 10s -> Reset WiFi`   | Kích hoạt chế độ cấu hình lại WiFi |
| `⏰ TimerX hết giờ -> Tắt Relay X` | Timer đếm về 0, tắt Relay tương ứng |
| `🔘 Nút X nhấn`                    | Nút vật lý được nhấn |

---

## 📌 Sơ đồ khối hệ thống

```text
┌─────────────────┐     UART      ┌──────────────┐
│   ATMega328P    │◄─────────────►│  Module Tuya │
│                 │   (D8/D9)     │   WiFi       │
│  ┌───────────┐  │               └──────────────┘
│  │ Buttons   │  │                      │
│  │ SW1/SW2/SW3│ │                      ▼
│  └───────────┘  │               ┌──────────────┐
│  ┌───────────┐  │               │  Cloud Tuya  │
│  │ Relays    │  │               │  (App Smart) │
│  │ R1/R2/R3  │  │               └──────────────┘
│  └───────────┘  │
│  ┌───────────┐  │
│  │ LED WiFi  │  │
│  └───────────┘  │
└─────────────────┘
```
## 📞 Hỗ trợ kỹ thuật
Mọi thắc mắc về firmware, vui lòng liên hệ:

📞 Điện thoại/Zalo: +84-915-898-345

✉️ Email: thanhcongted.info@gmail.com

🌐 Website: https://thanhcongted.com

## © Bản quyền
Phát triển bởi đội ngũ kỹ sư Việt Nam
Sản phẩm được thiết kế phù hợp với nhu cầu và thói quen sử dụng tại thị trường nội địa.# 💻 THANHCONGTED – Code ATMega328P + Tuya Platform

> **Mô tả:** Code dùng ATMega328P + Tuya Platform. Chỉ giữ: Button + Relay + LED WiFi. Xử lý 6 DP: 3 Switch (Bool) + 3 Timer (Value 0–86400s). BUTTON_SW1 giữ 10s → Reset WiFi Tuya (SMART_CONFIG).

---

## 📌 Sơ đồ chân (Pinout) – TED168-V2

| Chân MCU | Định nghĩa | Chức năng |
|----------|------------|-----------|
| D5  | `RELAY_1`  | Relay cho Switch 1 |
| D6  | `RELAY_2`  | Relay cho Switch 2 |
| D7  | `RELAY_3`  | Relay cho Switch 3 |
| D10 | `LED_WIFI` | LED báo trạng thái WiFi |
| D12 | `BUTTON_1` | Nút nhấn Switch 1 (kiêm nút reset WiFi) |
| D14 | `BUTTON_2` | Nút nhấn Switch 2 (nút LEARN) |
| D11 | `BUTTON_3` | Nút nhấn Switch 3 (SENSOR_CUA) |
| D8  | UART RX    | SoftwareSerial RX – Kết nối module Tuya |
| D9  | UART TX    | SoftwareSerial TX – Kết nối module Tuya |

---

## 📊 Data Point (DP) Tuya

| DP ID | Định nghĩa | Kiểu dữ liệu | Mô tả |
|-------|------------|--------------|-------|
| 1 | `DPID_SWITCH_1`    | BOOL  | Trạng thái Switch 1 (0/1) |
| 2 | `DPID_SWITCH_2`    | BOOL  | Trạng thái Switch 2 (0/1) |
| 3 | `DPID_SWITCH_3`    | BOOL  | Trạng thái Switch 3 (0/1) |
| 7 | `DPID_COUNTDOWN_1` | VALUE | Timer đếm ngược Switch 1 (0–86400s) |
| 8 | `DPID_COUNTDOWN_2` | VALUE | Timer đếm ngược Switch 2 (0–86400s) |
| 9 | `DPID_COUNTDOWN_3` | VALUE | Timer đếm ngược Switch 3 (0–86400s) |

---

## ⚙️ Thông số kỹ thuật Firmware

- **Vi điều khiển:** ATMega328P Arduino
- **Product ID (PID):** `ekdehkpnjp8squlk`
- **Phiên bản MCU:** `2.1.0`
- **UART Tuya:** SoftwareSerial @ 9600 bps (RX=D8, TX=D9)
- **UART Debug:** Hardware Serial @ 115200 bps
- **Thời gian nhấn giữ Reset WiFi:** 10 giây (BUTTON_SW1)
- **Thời gian debounce nút nhấn:** 25 ms
- **Chu kỳ vòng lặp chính (loop):** 10 ms

---

## 🔄 Logic hoạt động Firmware

### 1. Điều khiển Relay
- Mỗi Switch có một Relay tương ứng, điều khiển qua DP Bool từ Tuya hoặc nút nhấn vật lý.
- Relay 1 & 3: **Active LOW** (mức thấp = BẬT)
- Relay 2: **Active HIGH** (mức cao = BẬT)

### 2. Timer đếm ngược
- Mỗi Switch có một Timer riêng (0–86400 giây).
- Khi Timer đếm về 0 → tự động tắt Relay tương ứng.
- Giá trị Timer được cập nhật lên Cloud mỗi giây.

### 3. LED WiFi
- **Đã kết nối Cloud:** LED tắt.
- **Chưa kết nối Cloud:** LED nháy 500ms/lần.

### 4. Reset WiFi
- Nhấn giữ **BUTTON_SW1 trong 10 giây** → kích hoạt chế độ `SMART_CONFIG` để cấu hình lại WiFi Tuya.
- Sau khi nhả nút → thoát chế độ reset.

### 5. Nút nhấn vật lý
- **BUTTON_SW1:** Nhấn ngắn → đảo trạng thái Relay 1. Nhấn giữ 10s → Reset WiFi.
- **BUTTON_SW2:** Nhấn ngắn → đảo trạng thái Relay 2 (nút LEARN).
- **BUTTON_SW3:** Nhấn ngắn → đảo trạng thái Relay 3 (SENSOR_CUA).

---

## 🧩 Cấu trúc Code

```text
├── Khai báo chân (Pin Definitions)
├── Khởi tạo UART Tuya (SoftwareSerial)
├── Khởi tạo Buttons (JC_Button)
├── Định nghĩa DP Tuya (dp_array)
├── Biến trạng thái (sw_state, timer)
├── Hàm điều khiển Relay (set_relay_1/2/3)
├── Hàm xử lý DP tải xuống (dp_process)
├── Hàm đẩy toàn bộ DP lên Cloud (dp_update_all)
├── Hàm đếm ngược Timer (timer_tick)
├── Hàm cập nhật LED WiFi (wifi_led_update)
├── Setup()
└── Loop()
```

## 📡 Giao thức Tuya

- **Module WiFi:** Tuya TED WiFi (kết nối qua UART)
- **Chế độ cấu hình:** `SMART_CONFIG`
- **Trạng thái WiFi:** `WIFI_CONN_CLOUD` (đã kết nối Cloud)
- **Cập nhật DP:** Sử dụng `mcu_dp_update()` với độ dài tương ứng:
  - Bool: 1 byte
  - Value: 4 byte

---

## ⚠️ Lưu ý khi nạp Firmware

1. Nạp code qua **ISP** (Dùng bootloader Arduino nếu chưa nạp) 
2. Sau code qua **UART** chân RX_TX của MCU.
3. Kiểm tra logic Relay trước khi kết nối tải thực tế (Active LOW/HIGH).
4. **PID Tuya** phải khớp với sản phẩm đã đăng ký trên Tuya IoT Platform.
5. **Phiên bản MCU** (`2.1.0`) phải khớp với cấu hình trên Cloud Tuya.

---

## 🐛 Debug qua Serial (115200 bps)

| Log | Ý nghĩa |
|-----|---------|
| `THANHCONGTED - Switch + Timer`   | Khởi động thành công |
| `✅ Đẩy data lên điện thoại`       | Đồng bộ toàn bộ DP lên Cloud |
| `✅ WiFi đã kết nối Cloud`         | Module Tuya đã kết nối thành công |
| `⏳ Đang cấu hình WiFi...`         | Module chưa kết nối Cloud |
| `🔁 Nhấn giữ 10s -> Reset WiFi`   | Kích hoạt chế độ cấu hình lại WiFi |
| `⏰ TimerX hết giờ -> Tắt Relay X` | Timer đếm về 0, tắt Relay tương ứng |
| `🔘 Nút X nhấn`                    | Nút vật lý được nhấn |

---

## 📌 Sơ đồ khối hệ thống

```text
┌─────────────────┐     UART      ┌──────────────┐
│   ATMega328P    │◄─────────────►│  Module Tuya │
│                 │   (D8/D9)     │   WiFi       │
│  ┌───────────┐  │               └──────────────┘
│  │ Buttons   │  │                      │
│  │ SW1/SW2/SW3│ │                      ▼
│  └───────────┘  │               ┌──────────────┐
│  ┌───────────┐  │               │  Cloud Tuya  │
│  │ Relays    │  │               │  (App Smart) │
│  │ R1/R2/R3  │  │               └──────────────┘
│  └───────────┘  │
│  ┌───────────┐  │
│  │ LED WiFi  │  │
│  └───────────┘  │
└─────────────────┘
```
## 📞 Hỗ trợ kỹ thuật
Mọi thắc mắc về firmware, vui lòng liên hệ:

📞 Điện thoại/Zalo: +84-915-898-345

✉️ Email: thanhcongted.info@gmail.com

🌐 Website: https://thanhcongted.com

## © Bản quyền
Phát triển bởi đội ngũ kỹ sư Việt Nam
Sản phẩm được thiết kế phù hợp với nhu cầu và thói quen sử dụng tại thị trường nội địa.
