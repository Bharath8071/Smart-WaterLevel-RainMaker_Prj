1. Clone the repository:
```
git clone https://github.com/Bharath8071/Smart-WaterLevel-RainMaker.git
cd Smart-WaterLevel-RainMaker
```

2. Open in Arduino IDE, install:

- ESP32 board manager
- ESP RainMaker library 
- SimpleTimer

3. Configure Wi-Fi provisioning keys:
```
const char *service_name = "PROV_1234RS8";
const char *pop = "12345RS8";
```

4. Upload code to your ESP32.
5. Connect via RainMaker App → Scan QR code → Monitor and control water level system remotely.
