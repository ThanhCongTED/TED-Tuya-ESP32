[Việt Nam](README.md) | [English](README_eng.md)

# Ví dụ Lấy dữ liệu Thời tiết

## Tổng quan

Ví dụ này minh họa cách tích hợp Dịch vụ Thời tiết Tuya IoT với thiết bị của bạn. Nó cho thấy cách lấy dữ liệu thời tiết từ đám mây Tuya và điều khiển một công tắc thông minh với đèn LED chỉ báo.

## Tính năng

- Lấy dữ liệu thời tiết từ Dịch vụ Thời tiết Tuya IoT
- Chức năng công tắc thông minh với điều khiển LED
- Tương tác người dùng qua nút nhấn
- Chỉ báo trạng thái LED (tắt, bật, nhấp nháy)
- Quản lý cấu hình mạng
- Giám sát bộ nhớ qua Ticker

## Yêu cầu phần cứng

- Bo mạch phát triển được Tuya hỗ trợ
- LED kết nối với chân LED_BUILTIN
- Nút nhấn kết nối với chân BUTTON_BUILTIN

## Cấu hình

Cấu hình các tham số sau trước khi nạp firmware:

```cpp
#define THANHCONGTED_UUID "uuidxxxxxxxxxxxxxxxx"      // UUID thiết bị của bạn
#define THANHCONGTED_AUTHKEY "xxxxxxxxxxxxxxxxxxxxxxxx" // Auth key của bạn
```

Product ID: `qhivvyqawogv04e4`

## Cấu hình chân

- `LED_BUILTIN`: LED trạng thái (bật khi mức LOW)
- `BUTTON_BUILTIN`: Nút nhấn người dùng (kích hoạt mức LOW)

## Cách hoạt động

1. **Khởi tạo**: Thiết lập logging, LED, nút nhấn và kết nối IoT
2. **Dịch vụ Thời tiết**: Khởi tạo dịch vụ TuyaIoTWeather
3. **Điều khiển LED**: Quản lý trạng thái LED dựa trên DP công tắc
4. **Xử lý nút nhấn**: Nhấn ngắn để bật/tắt công tắc, nhấn giữ để xóa thiết bị
5. **Demo Thời tiết**: Minh họa việc lấy dữ liệu thời tiết
6. **Giám sát Heap**: Báo cáo heap trống mỗi 5 giây

## Các Data Point

- `DPID_SWITCH` (1): Kiểu Boolean - Trạng thái công tắc (điều khiển LED)

## Điều khiển nút nhấn

- **Nhấn ngắn**: Bật/tắt trạng thái công tắc/LED
- **Nhấn giữ (3 giây)**: Xóa thiết bị khỏi nền tảng Tuya IoT

## Trạng thái LED

- **Tắt**: Công tắc đang tắt
- **Bật**: Công tắc đang bật
- **Nhấp nháy (500ms)**: Thiết bị đang ở chế độ liên kết (binding)

## Cách sử dụng

1. Nạp firmware vào thiết bị của bạn
2. Mở Serial Monitor ở tốc độ 115200 baud
3. Thiết bị kết nối đến nền tảng Tuya IoT
4. Dùng nút nhấn để điều khiển LED hoặc dùng ứng dụng Tuya Smart
5. Dữ liệu thời tiết sẽ được lấy tự động

## Dịch vụ Thời tiết

Ví dụ bao gồm hàm `weatherGetDemo()` để minh họa việc lấy dữ liệu thời tiết. Bạn có thể tùy chỉnh hàm này để:
- Lấy thời tiết hiện tại
- Lấy dự báo thời tiết
- Lấy nhiệt độ, độ ẩm, v.v.
- Hiển thị thông tin thời tiết

## Các sự kiện được xử lý

- `TUYA_EVENT_BIND_START`: Thiết bị vào chế độ liên kết
- `TUYA_EVENT_ACTIVATE_SUCCESSED`: Kích hoạt thiết bị hoàn tất
- `TUYA_EVENT_MQTT_CONNECTED`: Đã kết nối đến đám mây Tuya
- `TUYA_EVENT_TIMESTAMP_SYNC`: Đồng bộ thời gian
- `TUYA_EVENT_DP_RECEIVE_OBJ`: Nhận lệnh DP

## Phụ thuộc

- Thư viện TuyaIoT
- Thư viện TuyaIoTWeather
- Thư viện tLed
- Thư viện Log
- Thư viện Ticker

## Lưu ý

- Đảm bảo có license thiết bị Tuya hợp lệ
- Dịch vụ thời tiết yêu cầu thiết bị phải online
- Giám sát heap trống giúp theo dõi mức sử dụng bộ nhớ
- Lớp tLed cung cấp điều khiển LED dễ dàng với chức năng nhấp nháy

## Xử lý sự cố

- Kiểm tra UUID và AuthKey nếu kết nối thất bại
- Xác minh cấu hình mạng
- Theo dõi output serial để tìm lỗi
- Kiểm tra heap trống nếu thiết bị hoạt động bất thường

## Tính năng nâng cao

- Tùy chỉnh việc lấy dữ liệu thời tiết
- Thêm nhiều DP hơn cho các điều khiển bổ sung
- Triển khai tự động hóa dựa trên thời tiết
- Lưu trữ lịch sử thời tiết

## 📞 Hỗ trợ kỹ thuật
Mọi thắc mắc về firmware, vui lòng liên hệ:

📞 Điện thoại/Zalo: +84-915-898-345

✉️ Email: thanhcongted.info@gmail.com

🌐 Website: https://thanhcongted.com

## © Bản quyền
Phát triển bởi đội ngũ kỹ sư Việt Nam
Sản phẩm được thiết kế phù hợp với nhu cầu và thói quen sử dụng tại thị trường nội địa.