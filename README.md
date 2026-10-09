# Home

![Spell Check](https://img.shields.io/github/actions/workflow/status/tuya/arduino-tuyaopen/spell-check.yml?style=plastic&label=Spell%20Check)

[Việt Nam](README.md) | [English](README_eng.md)

# Giới thiệu

arduino-tuyaopen là một framework phát triển IoT mã nguồn mở do Tuya cung cấp cho nền tảng Arduino, cho phép nhiều lập trình viên Arduino nhanh chóng phát triển các thiết bị IoT dựa trên nền tảng đám mây Tuya và hiện thực hóa việc điều khiển thông minh từ xa đối với thiết bị.

arduino-tuyaopen được xây dựng dựa trên [TuyaOpen](https://github.com/tuya/TuyaOpen), một framework phát triển IoT đa chip và đa hệ điều hành. Nó được thiết kế dựa trên Giao diện Southbound chung (Generic Southbound Interface) và hỗ trợ các giao thức truyền thông như Bluetooth, Wi-Fi và Ethernet. Nó cung cấp các chức năng cốt lõi của phát triển IoT, bao gồm cấu hình mạng, kích hoạt, điều khiển và nâng cấp. Đồng thời, nó còn có khả năng tuân thủ bảo mật mạnh mẽ, bao gồm xác thực thiết bị, mã hóa dữ liệu và mã hóa truyền thông, đáp ứng các yêu cầu tuân thủ dữ liệu của nhiều quốc gia và khu vực trên thế giới.

Các sản phẩm IoT được phát triển dựa trên TuyaOpen có thể sử dụng các chức năng của component tuya_cloud_service để tận dụng hệ sinh thái mạnh mẽ do TuyaAPP, các dịch vụ đám mây cung cấp, và có thể liên kết với các thiết bị Power By Tuya.

Đồng thời, TuyaOpen sẽ tiếp tục mở rộng, cung cấp thêm các chức năng truy cập nền tảng đám mây, cũng như các chức năng về giọng nói, video và nhận diện khuôn mặt, và arduino-tuyaopen cũng sẽ được cập nhật để hỗ trợ nhiều chức năng hơn.

# Các chip được hỗ trợ

Hiện tại, dự án hỗ trợ các chip T2, T3 và TUYA_T5AI.

| Chip |                          Datasheet                           |
| :--: | :----------------------------------------------------------: |
|  T2  | [T2](https://developer.tuya.com/en/docs/iot/T2-U-module-datasheet?id=Kce1tncb80ldq) |
|  T3  | [T3](https://developer.tuya.com/en/docs/iot/T3-U-Module-Datasheet?id=Kdd4pzscwf0il) |
|  TUYA_T5AI  | [TUYA_T5AI](https://developer.tuya.com/en/docs/iot/T5-E1-Module-Datasheet?id=Kdar6hf0kzmfi) |
|  LN882H  | LN882H |
|  ESP32  | [ESP32](https://www.espressif.com/en/products/socs/esp32/resources) |

# Các hệ điều hành được hỗ trợ

| Chip | Windows  | Linux |  macOS   |
| :--: | :------: | :---: | :------: |
|  T2  |   Hỗ trợ   | Hỗ trợ  | Tạm thời chưa hỗ trợ |
|  T3  |   Hỗ trợ   | Hỗ trợ  | Hỗ trợ |
|  TUYA_T5AI  |   Hỗ trợ   | Hỗ trợ  | Hỗ trợ |
|  LN882H  |   Hỗ trợ   | Hỗ trợ  | Hỗ trợ |
|  ESP32  |   Hỗ trợ   | Hỗ trợ  | Hỗ trợ |

> Lưu ý: Một số chip chưa được hỗ trợ trên một số hệ điều hành nhất định, chúng tôi đang nỗ lực để hỗ trợ chúng, hãy chờ đón nhé!

# Cách sử dụng arduino-tuyaopen

Bạn nên cài đặt và sử dụng phiên bản mới nhất của Arduino IDE 2, có thể tải xuống từ trang chủ chính thức của Arduino [arduino.cc](https://www.arduino.cc/en/software). Tất cả các công việc biên dịch, nạp và kiểm thử của dự án này đều được thực hiện trên Arduino IDE 2.

+ Sao chép địa chỉ quản lý bo mạch phát triển sau:

  ```
  https://github.com/tuya/arduino-tuyaopen/releases/download/global/package_tuya_open_index.json
  ```


+ Mở Arduino IDE 2 và nhấp vào "File" -> "Preferences" để mở cửa sổ tùy chọn.

  ![Preferences](https://images.tuyacn.com/fe-static/docs/img/581335e7-e012-4895-aece-7af21d00bbf5.png)

+ Trong trường "Other Board Manager URLs", nhập địa chỉ quản lý bo mạch phát triển ở trên.

  ![BoardManagerURL](https://images.tuyacn.com/fe-static/docs/img/cc3f4fa3-3fd6-458a-af90-a04b49225714.png)

+ Trong "Board Manager", tìm kiếm "TED Tuya ESP32" và cài đặt phiên bản mới nhất.

## Cách sử dụng arduino-tuyaopen để kết nối đám mây

+ [Kết nối với Nền tảng IoT Tuya](./libraries/TuyaIoT/examples/quickStart/README.md)

## Giới thiệu phần cứng cho Bo mạch phát triển

+ [Bo mạch phát triển T2-U](https://developer.tuya.com/cn/docs/iot/t2-u-board?id=Kce6cq9e9vlmv)
+ [Datasheet Module T3-U](https://developer.tuya.com/cn/docs/iot/T3-U-Module-Datasheet?id=Kdd4pzscwf0il)
+ [Datasheet Module T5-E1](https://developer.tuya.com/en/docs/iot/T5-E1-Module-Datasheet?id=Kdar6hf0kzmfi)

## Hỗ trợ
Trong quá trình sử dụng dự án này, nếu bạn gặp bất kỳ vấn đề gì hoặc có yêu cầu, ý tưởng về chức năng mới, bạn có thể giao lưu và trao đổi ý kiến thông qua các issue trong dự án này.