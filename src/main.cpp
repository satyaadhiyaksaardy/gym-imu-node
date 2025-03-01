#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

#define LED_PIN D2          // Green LED connected to D2

// BLE Service and Characteristic UUIDs
BLEService imuService("xxx");
BLECharacteristic imuDataCharacteristic(
  "xxx",
  BLERead | BLENotify,
  256
);

// Timing variables for data transmission
unsigned long previousCollectMillis = 0;
unsigned long previousSendMillis = 0;
const unsigned long sendInterval = 20;    // 50Hz
const unsigned long collectInterval = 10; // 100Hz

// LED blinking variables for advertising state
unsigned long previousBlinkMillis = 0;
const unsigned long blinkInterval = 500; // 500ms blink interval
bool ledState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // Initialize LED off

  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("BLE initialization failed!");
    while (1);
  }

  // Initialize IMU
  if (!IMU.begin()) {
    Serial.println("IMU initialization failed!");
    while (1);
  }

  // Setup BLE service and start advertising
  BLE.setLocalName("GymTrackerBLE");
  BLE.setAdvertisedService(imuService);
  imuService.addCharacteristic(imuDataCharacteristic);
  BLE.addService(imuService);
  BLE.advertise();
  Serial.println("BLE active - Waiting for connection...");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to: ");
    Serial.println(central.address());
    digitalWrite(LED_PIN, HIGH); // Solid LED when connected

    while (central.connected()) {
      unsigned long currentMillis = millis();
      static float ax = 0, ay = 0, az = 0;
      static float gx = 0, gy = 0, gz = 0;
      static float mx = 0, my = 0, mz = 0;

      // Collect sensor data at 100Hz
      if (currentMillis - previousCollectMillis >= collectInterval) {
        previousCollectMillis = currentMillis;

        if (IMU.accelerationAvailable()) IMU.readAcceleration(ax, ay, az);
        if (IMU.gyroscopeAvailable()) IMU.readGyroscope(gx, gy, gz);
        if (IMU.magneticFieldAvailable()) IMU.readMagneticField(mx, my, mz);
      }

      // Send data at 50Hz
      if (currentMillis - previousSendMillis >= sendInterval) {
        previousSendMillis = currentMillis;

        char jsonBuffer[256];
        snprintf(jsonBuffer, sizeof(jsonBuffer),
          "{\"accelerometer\":[%.2f,%.2f,%.2f],"
          "\"gyroscope\":[%.2f,%.2f,%.2f],"
          "\"magnetometer\":[%.2f,%.2f,%.2f]}",
          ax, ay, az, gx, gy, gz, mx, my, mz);

        Serial.println(jsonBuffer);
        imuDataCharacteristic.writeValue(jsonBuffer);
      }

      BLE.poll(); // Maintain BLE connection
    }

    // Disconnection cleanup
    digitalWrite(LED_PIN, LOW);
    Serial.println("Disconnected");
  } else {
    // Blink LED when advertising
    unsigned long currentMillis = millis();
    if (currentMillis - previousBlinkMillis >= blinkInterval) {
      previousBlinkMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }
  }
}