**1. Clone the repository:**
```
git clone https://github.com/Bharath8071/Smart-WaterLevel-RainMaker.git
cd Smart-WaterLevel-RainMaker
```

**2. Open in Arduino IDE, install:**

- ESP32 board manager
- [ESP RainMaker library](https://github.com/espressif/esp-rainmaker)
- SimpleTimer

**3. Configure Wi-Fi provisioning keys:**
```
const char *service_name = "PROV_1234RS8";
const char *pop = "12345RS8";
```

**4. Upload code to your ESP32.**
**5. Connect via RainMaker App → Scan QR code → Monitor and control water level system remotely.**

## Note:

- The ESP RainMaker library may not appear in the Arduino IDE Library Manager.
Install it directly from the RainMaker GitHub repository
- In Arduino IDE → Tools → Board, select your ESP32 development board (e.g., DOIT ESP32 DEVKIT V1).
  - The Partition Scheme option should include “RAINMAKER”.
  - Important: On newer ESP32 Arduino core versions, this option may not appear. It is usually available in older ESP32 board versions.The version I use `ESP DEV MODEL- 2.0.14`.
  - Make sure to double-check your board version  
