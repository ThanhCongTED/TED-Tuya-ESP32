# Home

![Spell Check](https://img.shields.io/github/actions/workflow/status/tuya/arduino-tuyaopen/spell-check.yml?style=plastic&label=Spell%20Check)

[Việt Nam](README.md) | [English](README_eng.md)

# Introduction

Ted Tuya ESP32 is an open-source IoT development framework provided by Tuya for the Arduino platform, allowing many Arduino developers to quickly develop IoT devices based on the Tuya cloud platform and realize remote intelligent control of devices.

IoT products developed based on TED_TUYA_ESP32 can use the functions of the tuya_cloud_service component to take advantage of the powerful ecosystem provided by TuyaAPP or SmartLife and cloud services, and can interconnect with Tuya devices.

At the same time, TED_Tuya_ESP32 will continue to expand, providing more cloud platform access functions, as well as voice, video, and face recognition functions, and TED_Tuya_ESP32 will also be updated to support more functions.

# Supported Chips

Currently, the project supports the ESP32 chip.

| Chip |                          Datasheet                           |
| :--: | :----------------------------------------------------------: |
|  ESP32  | [ESP32](https://www.espressif.com/en/products/socs/esp32/resources) |

# Supported Operating Systems

| Chip | Windows  | Linux |  macOS   |
| :--: | :------: | :---: | :------: |
|  ESP32  |   Supported   | Supported  | Supported |

> Note: Some chips are not yet supported on certain operating systems. We are working hard to support them, so stay tuned!

# How to use TED_TUYA_ESP32

It is recommended to install and use the latest version of Arduino IDE 2, which can be downloaded from the official Arduino website [arduino.cc](https://www.arduino.cc/en/software). All compilation, flashing, and testing of this project are carried out on Arduino IDE 2.

+ Copy the following development board management address:

  ```
  https://github.com/ThanhCongTED/TED-Tuya-ESP32/releases/download/global/package_ted_tuya_esp32_index.json
  ```


+ Open Arduino IDE 2 and click "File" -> "Preferences" to open the preferences window.

  ![Preferences](https://github.com/ThanhCongTED/TED-Tuya-ESP32/blob/main/image/Preferences.jpg)

+ In the "Other Board Manager URLs" field, enter the above development board management address.

  ![BoardManagerURL](https://github.com/ThanhCongTED/TED-Tuya-ESP32/blob/main/image/BoardManager.jpg)

+ In the "Board Manager", search for "TED Tuya ESP32" and install the latest version.

## How to use arduino-ted-tuya for cloud connection

+ [Connect to Tuya IoT Platform](./tuya_open/libraries/TuyaIoT/examples/quickStart/README_eng.md)

## Hardware Introduction for Development Board

+ [TED-ESP32 Development Board](https://thanhcongted.com/)


## 📞 Technical Support
For any questions about the firmware, please contact:

📞 Phone/Zalo: +84-915-898-345

✉️ Email: thanhcongted.info@gmail.com

🌐 Website: https://thanhcongted.com

## © Copyright
Developed by a team of Vietnamese engineers
The product is designed to suit the needs and usage habits of the domestic market.