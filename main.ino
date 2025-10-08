#include "RMaker.h"
#include "WiFi.h"
#include "WiFiProv.h"
#include <SimpleTimer.h>

const char *service_name = "WIFI NAME";
const char *pop = "WIFI_PASSWORD";

// define the Chip Id
uint32_t espChipId = 5;

// define the Node Name
char nodeName[] = "ESP32_Relay_1S";

const int buzzerPin = 33;
const int relay1 = 5;
const int relay2 = 4;
const int relay3 = 2;
const int sensor2 = 32;
const int sensor1 = 34;

int minSensorValue = 3500;
String relayState = "false";
unsigned long startMicros;
unsigned long duration = 12000000;   // Duration in microseconds (2 minutes)

unsigned long currentMicros;

// define the Device Names
char deviceName_1[] = "SWITCH_1";

// define the GPIO connected with Relays and switches
static uint8_t wifiLed = 2;
static uint8_t gpio_reset = 0;

// Relay State
bool relay1State = LOW; // Define integer to remember the toggle state for relay 1
bool initial_check = HIGH;
SimpleTimer Timer;

// The framework provides some standard device types like switch, lightbulb, fan, temperature sensor.
static Switch my_switch1(deviceName_1);

void sysProvEvent(arduino_event_t *sys_event) {
    switch (sys_event->event_id) {
        case ARDUINO_EVENT_PROV_START:
#if CONFIG_IDF_TARGET_ESP32
            Serial.printf("\nProvisioning Started with name \\%s\\ and PoP \\%s\\ on BLE\n", service_name, pop);
            printQR(service_name, pop, "ble");
#else
            Serial.printf("\nProvisioning Started with name \\%s\\ and PoP \\%s\\ on SoftAP\n", service_name, pop);
            printQR(service_name, pop, "softap");
#endif        
        break;
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            Serial.printf("\nConnected to Wi-Fi!\n");
            delay(5000);
            //my_switch1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false); 
            Serial.println("->2entered"); 
            esp_rmaker_raise_alert("Semmipan Is Activated");
            manualSwitchControl();
            digitalWrite(wifiLed, true);
        break;
    }
}


void manualSwitchControl() {
    // Get the device (my_switch1 is your existing device from RainMaker)
    Device *mockDevice = &my_switch1;

    // Replace this line with the correct way to access the parameter
    esp_rmaker_param_t *mockParam = mockDevice->getParamByName(ESP_RMAKER_DEF_POWER_NAME); // Hypothetical method

    if (mockParam == nullptr) {
        Serial.println("Parameter not found!");
        return; // Handle the case where the parameter does not exist
    }

    // Get the current state from the parameter
    esp_rmaker_param_val_t *currentStatePtr = esp_rmaker_param_get_val(mockParam); // Get a pointer to the current state
    if (currentStatePtr == nullptr) {
        Serial.println("Failed to get current state!");
        return; // Handle error if needed
    }

    // Use the dereferenced pointer to get the current state
    param_val_t mockValue;
    mockValue.val.b = !currentStatePtr->val.b; // Toggle the current state (for example)

    // Call the write_callback manually with the device, parameter, and new value
    write_callback(mockDevice, reinterpret_cast<Param*>(mockParam), mockValue, nullptr, nullptr);
}


void write_callback(Device *device, Param *param, const param_val_t val, void *priv_data, write_ctx_t *ctx) {
    const char *device_name = device->getDeviceName();
    const char *param_name = param->getParamName();

    if (strcmp(device_name, deviceName_1) == 0) {
        Serial.printf("Lightbulb = %s\n", val.val.b ? "true" : "false");
      
        if (strcmp(param_name, "Power") == 0) {
            Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
            relay1State = val.val.b;
            Serial.println("relay1State->->->");
            Serial.println(relay1State);
            rmaker();
            param->updateAndReport(val);
        }
    }
}

void sensor() {    
    // Read sensor values
    int sensor1Value = analogRead(sensor1);
    int sensor2Value = analogRead(sensor2);
    //Serial.printf("sensor1Value:");
    //Serial.print(sensor1Value);
    //Serial.printf("sensor2Value:");
    // Serial.print(sensor2Value);

    // Control relays based on sensor values
    if (sensor1Value <= 0 && sensor2Value <= 0 && relayState == "false") {
        digitalWrite(relay1, HIGH);
        digitalWrite(relay2, HIGH);
        Serial.println("Relay 1,2 ON");
        delay(5000);
        digitalWrite(relay1, LOW);
        digitalWrite(relay2, LOW);
        Serial.println("Relay 1,2 OFF");
        delay(5000);
        startMicros = micros();
        relayState = "true";
        my_switch1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, true); // Report updated state
        delay(1000);
        Serial.println("1-> ON");
    }

    currentMicros = micros();
    if (currentMicros - startMicros >= duration && sensor1Value <= 0 && relayState == "true") {
        digitalWrite(relay3, LOW);
        digitalWrite(buzzerPin, HIGH);
        Serial.println("Relay 3 OFF and Buzzer ON");
        delay(5000);
        digitalWrite(relay3, HIGH);
        digitalWrite(buzzerPin, LOW);
        Serial.println("Relay 3 ON and Buzzer OFF");
        delay(5000);  
        relayState = "NULL";
        my_switch1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false); // Report updated state
        esp_rmaker_raise_alert("❌NOTIFICATION ALERT 🔔❌");
        startMicros = 0;
        delay(1000);
        Serial.println("3-> NULL");
    }

    if (sensor1Value >= minSensorValue && sensor2Value >= minSensorValue && relayState == "true") {
        digitalWrite(relay3, LOW);
        Serial.println("Relay 3 OFF");
        delay(5000);
        digitalWrite(relay3, HIGH);
        Serial.println("Relay 3 ON");
        delay(5000);
        relayState = "false";
        my_switch1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false); // Report updated state
        delay(1000);
        Serial.println("2-> OFF");
    }
}

void rmaker() {
    if (relay1State && relayState == "false" || relayState == "NULL") {
        digitalWrite(relay1, HIGH);
        digitalWrite(relay2, HIGH);
        Serial.println("Relay 1,2 R ON");
        delay(5000);
        digitalWrite(relay1, LOW);
        digitalWrite(relay2, LOW);
        Serial.println("Relay 1,2 R OFF");
        delay(5000);
        startMicros = micros();
        relayState = "true";

    } else if (!relay1State && relayState == "true") {
        digitalWrite(relay3, LOW);
        Serial.println("Relay 3 R OFF");
        delay(5000);
        digitalWrite(relay3, HIGH);
        Serial.println("Relay 3 R ON");
        delay(5000);
        relayState = "NULL";
    }

    delay(100);
}

void setup() {
    Serial.begin(115200);

    // Set the Relays GPIOs as output mode
    pinMode(wifiLed, OUTPUT);
    
    // Configure the input GPIOs
    pinMode(gpio_reset, INPUT);
    
    // Write to the GPIOs the default state on booting
    digitalWrite(wifiLed, LOW);

    // Configure pins
    pinMode(buzzerPin, OUTPUT);
    pinMode(sensor1, INPUT);
    pinMode(sensor2, INPUT);
    pinMode(relay1, OUTPUT);
    pinMode(relay2, OUTPUT);
    pinMode(relay3, OUTPUT);

    // Initialize relays
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
    digitalWrite(relay3, HIGH);

    Node my_node;
    my_node = RMaker.initNode(nodeName);

    // Standard switch device
    my_switch1.addCb(write_callback);

    // Add switch device to the node   
    my_node.addDevice(my_switch1);
    
    Timer.setInterval(2000);

    // Enable OTA and scheduling
    RMaker.enableOTA(OTA_USING_PARAMS);
    RMaker.enableTZService();
    RMaker.enableSchedule();

    // Service Name
    for (int i = 0; i < 15; i = i + 7) {
        espChipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    }

    Serial.printf("\nChip ID:  %d Service Name: %s\n", espChipId, service_name);

    Serial.printf("\nStarting ESP-RainMaker\n");
    RMaker.start();

    WiFi.onEvent(sysProvEvent);
#if CONFIG_IDF_TARGET_ESP32
    WiFiProv.beginProvision(WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BTDM, WIFI_PROV_SECURITY_1, pop, service_name);
#else
    WiFiProv.beginProvision(WIFI_PROV_SCHEME_SOFTAP, WIFI_PROV_SCHEME_HANDLER_NONE, WIFI_PROV_SECURITY_1, pop, service_name);
#endif
    my_switch1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false); 
    Serial.println("->1entered");
    delay(2000);

}

void loop() {
    // Read GPIO0 (external button to reset device)
    if (digitalRead(gpio_reset) == LOW) {
        Serial.printf("Reset Button Pressed!\n");
        // Key debounce handling
        delay(100);
        int startTime = millis();
        while (digitalRead(gpio_reset) == LOW) delay(50);
        int endTime = millis();

        if ((endTime - startTime) > 5000) {
            Serial.printf("Reset to factory.\n");
            RMakerFactoryReset(2);
        } else if ((endTime - startTime) > 3000) {
            Serial.printf("Reset Wi-Fi.\n");
            RMakerWiFiReset(2);
        }
    }
    delay(100);

    if (WiFi.status() != WL_CONNECTED) {
        digitalWrite(wifiLed, false);
    } else {
        digitalWrite(wifiLed, true);
        if (Timer.isReady()) {
            Timer.reset();
        }
    }
    // Serial.println("->bug<-");
    sensor();

}
