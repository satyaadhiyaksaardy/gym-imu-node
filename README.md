# GymTrackerBLE Project

## Overview
This repository contains source code for a BLE-based motion sensor device using Arduino. The device reads data from an IMU (Inertial Measurement Unit) sensor and sends it over Bluetooth Low Energy (BLE).

The project is intended to work with the Nano 33 BLE board, which includes the LSM9DS1 IMU sensor.

## Directory Structure
```
.
├── README.md
├── src/
│   └── main.cpp            - Main source file for Arduino sketch.
└── platformio.ini          - PlatformIO project configuration file.
```

## Header Files
Header files are intended to be used to share declarations and macros between several source files. Currently, there is one header file `header.h` in the `src` folder.

## Setup Instructions

### Prerequisites
- Arduino IDE or PlatformIO installed on your system.
- Nano 33 BLE board with an appropriate USB cable.

### Building the Project

#### Using PlatformIO
1. Install [PlatformIO](https://platformio.org/install).
2. Open this project in PlatformIO Home and run the build process by clicking `Build` from the toolbar.

## Source Code Overview

### main.cpp
The source code is organized into two main functions: `setup()` and `loop()`. Below is a brief overview of each function:

#### setup()
- Initializes serial communication.
- Attempts to initialize BLE and IMU sensors.
- Configures the BLE device with a local name and sets up an advertised service.

#### loop()
- Waits for a BLE central (device) connection.
- Reads data from accelerometer, gyroscope, and magnetometer if available.
- Formats sensor data into JSON and sends it over BLE.

## Dependencies
The project depends on the following libraries:
- Arduino_LSM9DS1: For communication with the LSM9DS1 IMU sensor.
- ArduinoBLE: For Bluetooth Low Energy functionality.

## Additional Documentation
For more information on using header files in C/C++, refer to the [GCC documentation](https://gcc.gnu.org/onlinedocs/cpp/Header-Files.html).
