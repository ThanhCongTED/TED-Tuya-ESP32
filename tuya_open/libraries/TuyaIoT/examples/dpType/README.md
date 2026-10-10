# Ví dụ dpType của TuyaIoT

## Tổng quan

Ví dụ này minh họa cách xử lý các loại Data Point (DP) khác nhau trong ứng dụng Tuya IoT. Nó cho thấy cách đọc và ghi nhiều loại DP bao gồm boolean, value, enum, string, bitmap và raw.

## Tính năng

- Xử lý nhiều loại DP (boolean, value, enum, string, bitmap, raw)
- Đọc giá trị DP từ đám mây Tuya
- Ghi giá trị DP lên đám mây Tuya
- Điều khiển nút nhấn để reset thiết bị
- Quản lý cấu hình mạng

## Yêu cầu phần cứng

- Bo mạch phát triển được Tuya hỗ trợ (ESP32, T2, T3, T5, v.v.)
- Nút nhấn kết nối với chân BUTTON_BUILTIN

## Các Data Point

Ví dụ định nghĩa các DP sau:

- `DPID_SWITCH` (20): Kiểu Boolean - Trạng thái công tắc
- `DPID_MODE` (21): Kiểu Enum - Chế độ hoạt động
- `DPID_BRIGHT` (22): Kiểu Value - Mức độ sáng
- `DPID_BITMAP` (101): Kiểu Bitmap - Nhiều cờ hiệu
- `DPID_STRING` (102): Kiểu String - Dữ liệu văn bản
- `DPID_RAW` (103): Kiểu Raw - Dữ liệu nhị phân

## Cấu hình

Trước khi nạp firmware, hãy cấu hình các tham số sau:

```cpp

const char *pid = "ekdehkpnjp8squlk";
const char *mcu_ver = "2.1.0";

// Thông tin xác thực Tuya
#define THANHCONGTED_UUID    "uuidxxxxxxxxxxxxxxxx" // Thay bằng UUID thiết bị của bạn
#define THANHCONGTED_AUTHKEY "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" // Thay bằng auth key của bạn

```

## Cách hoạt động

1. **Khởi tạo**: Khởi tạo giao tiếp serial và logging
2. **Thiết lập License**: Đọc license của bo mạch hoặc dùng thông tin xác thực cứng
3. **Kết nối IoT**: Kết nối đến nền tảng Tuya IoT
4. **Xử lý DP**: Nhận lệnh DP từ đám mây và báo cáo trạng thái trở lại
5. **Điều khiển nút nhấn**: Nhấn giữ nút để xóa thiết bị

## Các thao tác DP

### Boolean (Công tắc)
```cpp
bool switchStatus = 0;
TuyaIoT.read(event, DPID_SWITCH, switchStatus);
TuyaIoT.write(DPID_SWITCH, switchStatus);
```

### Value (Số nguyên)
```cpp
int brightValue = 0;
TuyaIoT.read(event, DPID_BRIGHT, brightValue);
TuyaIoT.write(DPID_BRIGHT, brightValue);
```

### Enum (Chế độ)
```cpp
uint32_t mode = 0;
TuyaIoT.read(event, DPID_MODE, mode);
TuyaIoT.write(DPID_MODE, mode);
```

### String
```cpp
char *strValue = NULL;
TuyaIoT.read(event, DPID_STRING, strValue);
TuyaIoT.write(DPID_STRING, strValue);
```

### Raw (Nhị phân)
```cpp
uint8_t *rawValue = NULL;
uint16_t len = 0;
TuyaIoT.read(event, DPID_RAW, rawValue, len);
TuyaIoT.write(DPID_RAW, rawValue, len);
```

## Cách sử dụng

1. Nạp firmware vào thiết bị của bạn
2. Mở Serial Monitor ở tốc độ 115200 baud
3. Thiết bị sẽ kết nối đến nền tảng Tuya IoT
4. Sử dụng ứng dụng Tuya Smart để điều khiển các DP
5. Theo dõi output serial để xem giá trị DP
6. Nhấn giữ nút (3 giây) để xóa thiết bị

## Điều khiển nút nhấn

- **Nhấn ngắn**: Không có hành động (có thể tùy chỉnh)
- **Nhấn giữ (3 giây)**: Xóa thiết bị khỏi nền tảng Tuya IoT

## Các sự kiện được xử lý

- `TUYA_EVENT_BIND_START`: Bắt đầu liên kết thiết bị
- `TUYA_EVENT_ACTIVATE_SUCCESSED`: Kích hoạt thiết bị thành công
- `TUYA_EVENT_TIMESTAMP_SYNC`: Đồng bộ thời gian
- `TUYA_EVENT_DP_RECEIVE_OBJ`: Nhận dữ liệu DP kiểu object
- `TUYA_EVENT_DP_RECEIVE_RAW`: Nhận dữ liệu DP kiểu raw

## Phụ thuộc

- Thư viện TuyaIoT
- Thư viện Log

## Lưu ý

- Đảm bảo bạn có license thiết bị Tuya hợp lệ
- Cấu hình Product ID chính xác cho loại thiết bị của bạn
- Ví dụ minh họa tất cả các loại DP phổ biến
- Các DP ID phải khớp với schema sản phẩm của bạn trên nền tảng Tuya IoT

## Xử lý sự cố

- Nếu thiết bị không kết nối, hãy kiểm tra UUID và AuthKey
- Kiểm tra Product ID khớp với sản phẩm của bạn trên nền tảng Tuya
- Đảm bảo các DP ID khớp với schema sản phẩm của bạn
- Kiểm tra output serial để xem log chi tiết

## Tìm hiểu thêm

- Hiểu cách định nghĩa DP trên nền tảng Tuya IoT
- Tìm hiểu về các loại DP khác nhau và trường hợp sử dụng của chúng
- Xem cách xử lý giao tiếp DP hai chiều

## 📞 Hỗ trợ kỹ thuật
Mọi thắc mắc về firmware, vui lòng liên hệ:

📞 Điện thoại/Zalo: +84-915-898-345

✉️ Email: thanhcongted.info@gmail.com

🌐 Website: https://thanhcongted.com

## © Bản quyền
Phát triển bởi đội ngũ kỹ sư Việt Nam
Sản phẩm được thiết kế phù hợp với nhu cầu và thói quen sử dụng tại thị trường nội địa.