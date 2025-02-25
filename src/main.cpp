#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

// Custom Service UUID (128-bit)
BLEService imuService("xxx");

// Characteristic UUID with increased value size for JSON data
BLECharacteristic imuDataCharacteristic(
  "xxx",
  BLERead | BLENotify,
  256  // Characteristic size for JSON payload
);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("Failed to initialize BLE!");
    while (1);
  }

  // Initialize IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  // Configure BLE device
  BLE.setLocalName("GymTrackerBLE");
  BLE.setAdvertisedService(imuService);
  imuService.addCharacteristic(imuDataCharacteristic);
  BLE.addService(imuService);

  // Start advertising
  BLE.advertise();
  Serial.println("BLE device active, waiting for connections...");
}

void loop() {
  // Wait for BLE central connection
  BLEDevice central = BLE.central();
  
  if (central) {
    Serial.print("Connected to: ");
    Serial.println(central.address());

    // While central is connected
    while (central.connected()) {
      // Initialize sensor values to 0 each iteration
      float ax = 0, ay = 0, az = 0;
      float gx = 0, gy = 0, gz = 0;
      float mx = 0, my = 0, mz = 0;

      // Read available sensor data
      if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(ax, ay, az);
      }
      if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(gx, gy, gz);
      }
      if (IMU.magneticFieldAvailable()) {
        IMU.readMagneticField(mx, my, mz);
      }

      // Create JSON-formatted string
      String jsonPayload = "{";
      jsonPayload += "\"accelerometer\":[" + String(ax, 2) + "," + String(ay, 2) + "," + String(az, 2) + "],";
      jsonPayload += "\"gyroscope\":[" + String(gx, 2) + "," + String(gy, 2) + "," + String(gz, 2) + "],";
      jsonPayload += "\"magnetometer\":[" + String(mx, 2) + "," + String(my, 2) + "," + String(mz, 2) + "]";
      jsonPayload += "}";

      // Update characteristic value and notify
      imuDataCharacteristic.writeValue(jsonPayload.c_str());

      // Short delay to prevent flooding
      delay(100);
    }

    Serial.println("Disconnected from central");
  }
}