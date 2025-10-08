# Smart Water Level Management using ESP RainMaker

This project is designed to automate water level monitoring and control using ESP32 and Espressif RainMaker. It can detect tank water levels using sensors, trigger alerts through RainMaker, and automatically control relays for motor operation.

It uses:
- RainMaker IoT Cloud (for remote monitoring and control)
- ESP32 (core controller)
- Analog water level sensors (detect levels)
- Buzzer and relays (for alert and motor control)

## Hardware Used
| Component |	Purpose|
|---|---|
| ESP32	| Main microcontroller (Wi-Fi + BLE) |
| Water Level Sensors |	Detect low and high water levels |
| Buzzer |	Alerts abnormal conditions |
| Relays (x3) |	Control motor and system valves |
| Wi-Fi LED	| Indicates connectivity status |

## Features

- Automatic motor ON/OFF control based on sensor readings
- RainMaker integration for real-time monitoring and alerts
- BLE-based provisioning for Wi-Fi setup
- Manual override using RainMaker switch
- Time-based alert system using SimpleTimer
- Buzzer alerts on abnormal conditions
- OTA and schedule enabled via RainMaker

## Connections
| ESP32Pin	| Connected To	| Function |
|---|---|---|
| 33	| Buzzer |	Sound alert |
| 5, 4, 2 |	Relays |	Control motor/valves |
| 32, 34	| Sensors	| Water level sensing |
|0	| Reset Button	| Wi-Fi / Factory reset |
| 2	| Wi-Fi LED	| Connectivity indicator |

## Software & Libraries

- ESP RainMaker SDK
- WiFi & WiFiProv libraries
- SimpleTimer
- Arduino IDE or PlatformIO
- 
## Working Principle
- The ESP32 connects to Wi-Fi using RainMaker provisioning.
- Two analog sensors measure water levels.
- When both sensors detect low levels → motor (Relay 1 & 2) turns ON.
- After filling or timeout → motor turns OFF, buzzer may alert abnormal behavior.
- All state changes are synced with RainMaker Cloud for remote view/control.
