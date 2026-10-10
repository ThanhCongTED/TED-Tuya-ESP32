[Việt Nam](README.md) | [English](README_eng.md)

# Bắt đầu nhanh với Tuya IoT

Tuya Cloud Application là một ứng dụng được cung cấp bởi nền tảng Tuya IoT. Với Tuya Cloud Application, các nhà phát triển có thể nhanh chóng triển khai điều khiển thiết bị từ xa, quản lý thiết bị và các chức năng khác.

Ví dụ `quickStart` minh họa một ví dụ công tắc đơn giản, đa nền tảng, đa hệ thống, hỗ trợ nhiều kết nối. Thông qua ứng dụng Tuya APP và Dịch vụ Đám mây Tuya, bạn có thể điều khiển từ xa đèn LED này.

## Tạo sản phẩm

Tạo một sản phẩm trên nền tảng [Tuya IoT](https://iot.tuya.com) và lấy PID của sản phẩm đã tạo.

Sau đó thay thế PID trong `quickStart` bằng PID bạn đã lấy được.

```c
const char *pid = "ekdehkpnjp8squlk";
const char *mcu_ver = "2.1.0";
```

## Xác nhận mã License của TED_TUYA_ESP32

Các sản phẩm được phát triển thông qua dự án này cần sử dụng mã license dành riêng cho Tuya Sử dụng mã license khác sẽ không thể kết nối đúng cách với Tuya Cloud.

Sửa mã license trong code như sau:

```c
// Ted license Tuya
#define THANHCONGTED_UUID    "uuidxxxxxxxxxxxxxxxx" // Thay bằng UUID thiết bị của bạn
#define THANHCONGTED_AUTHKEY "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" // Thay bằng auth key của bạn
```

Nếu bạn chưa có mã Ted license Tuya, bạn có thể lấy nó thông qua các cách sau:

+ Mỗi thiết bị cần 1 mã Ted license Tuya để giao tiếp Tuya Cloud  [TED_Tuya platform](https://thanhcongted.com/webinstaller/tuya.html).

## 📞 Hỗ trợ kỹ thuật
Mọi thắc mắc về firmware, vui lòng liên hệ:

📞 Điện thoại/Zalo: +84-915-898-345

✉️ Email: thanhcongted.info@gmail.com

🌐 Website: https://thanhcongted.com

## © Bản quyền
Phát triển bởi đội ngũ kỹ sư Việt Nam
Sản phẩm được thiết kế phù hợp với nhu cầu và thói quen sử dụng tại thị trường nội địa.

