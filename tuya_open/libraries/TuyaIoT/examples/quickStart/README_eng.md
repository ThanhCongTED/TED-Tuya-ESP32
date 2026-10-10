[Việt Nam](README.md) | [English](README_eng.md)

# Tuya IoT Quick Start

Tuya Cloud Application is an application provided by the Tuya IoT platform. With Tuya Cloud Application, developers can quickly implement remote device control, device management, and other functions.

The `quickStart` demonstrates a simple, cross-platform, cross-system switch example that supports multiple connections. Through the Tuya APP and Tuya Cloud Service, you can remotely control this LED.

## Product Creation

To create a product on the [Tuya IoT](https://iot.tuya.com) platform and obtain the PID of the created product.

Then replace the PID in `quickStart` with the PID you obtained.

```c
const char *pid = "ekdehkpnjp8squlk";
const char *mcu_ver = "2.1.0";
```

## Confirm TED_TUYA_ESP32 License Code

Products developed through this project need to use the TED_TUYA_ESP32 license code. Using other license codes will not connect to the Tuya Cloud properly.

Modify the license code in the code as follows:

```c

// Tuya license
#define THANHCONGTED_UUID    "uuidxxxxxxxxxxxxxxxx" // Thay bằng UUID thiết bị của bạn
#define THANHCONGTED_AUTHKEY "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" // Thay bằng auth key của bạn

```

If you do not have a TED_TUYA_ESP32 license code, you can obtain it through the following methods:

+ Each device requires a TED Tuya license code to communicate with Tuya Cloud. [TED_Tuya platform](https://thanhcongted.com/webinstaller/tuya.html)


## 📞 Technical Support
For any questions about the firmware, please contact:

📞 Phone/Zalo: +84-915-898-345

✉️ Email: thanhcongted.info@gmail.com

🌐 Website: https://thanhcongted.com

## © Copyright
Developed by a team of Vietnamese engineers
The product is designed to suit the needs and usage habits of the domestic market.