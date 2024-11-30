# Remote Farm Equipment Tracking and Management

This project is a comprehensive solution for tracking and managing farm equipment in real-time using **IoT**. The system integrates various sensors such as **GPS**, **MPU6050 (Accelerometer/Gyroscope)**, **Battery and Fuel Level Sensors**, and **RFID/NFC tagging**. It also supports real-time data visualization via the **Blynk platform** and **LoRaWAN** communication for extended range.

## Project Overview

The goal of this project is to help farm managers monitor the status of their equipment efficiently. It tracks motion, battery levels, fuel levels, and location data, while also providing real-time alerts for critical conditions such as low battery or fuel levels.

The system uses **Wi-Fi** to communicate with the Blynk platform for monitoring and alerting, and **LoRaWAN** is included for long-range communication (simulated in the project).

View the project in action:  
[**Remote Farm Equipment Tracking and Management on Wokwi**](https://wokwi.com/projects/415975220867492865)

## Features

- **Motion Detection**: Uses the **MPU6050** sensor to detect movement and activity based on acceleration and gyroscope readings.
- **Battery and Fuel Monitoring**: Monitors and alerts when battery or fuel levels drop below a threshold.
- **GPS Tracking**: Provides the real-time location of equipment using simulated GPS coordinates.
- **Data Visualization**: Displays sensor data (battery level, fuel level, motion status, etc.) on a Blynk dashboard.
- **Long-range Communication**: Simulates LoRaWAN communication for sending data over long distances (not fully implemented in this simulation).
- **Real-time Alerts**: Notifies the user of critical levels (low battery/fuel) with visual indicators (LEDs) and messages on Blynk.

## Sensors and Components

- **MPU6050 (Accelerometer/Gyroscope)**: Used for motion detection.
- **Battery Sensor**: Monitors battery levels.
- **Fuel Sensor**: Monitors fuel levels.
- **GPS**: Provides the location of the equipment (simulated in this project).
- **RFID/NFC Tags**: Used for identifying equipment (simulated in this project).
- **LEDs**: Indicate low battery and fuel levels.
- **Wi-Fi Module (ESP32)**: For communication with the Blynk platform.
- **LoRa Module**: For long-range communication (simulated in the code).

## Setup Instructions

1. **Hardware Setup**:
   - Connect the **MPU6050** sensor to the ESP32 for motion tracking.
   - Wire the **Battery** and **Fuel Sensors** to analog input pins of the ESP32.
   - Connect an **RFID/NFC** module if applicable (simulated in the project).
   - Use an **LED** indicator for battery and fuel warnings.

2. **Blynk Setup**:
   - Create a Blynk project and generate the **Auth Token**.
   - Define the **BLYNK_TEMPLATE_ID** and **BLYNK_TEMPLATE_NAME** in your code.
   - Configure the **Wi-Fi credentials** in the code.

3. **Code Configuration**:
   - Clone or download this repository to your Arduino IDE.
   - Upload the code to your ESP32 board.
   - Make sure all components are correctly connected, and the Wi-Fi credentials are properly set.

4. **Testing**:
   - Monitor the Blynk app for real-time data updates.
   - Simulate different scenarios (e.g., low battery, motion) to see the system’s response.

## Group Members

Our team is made up of four dedicated members who contributed to various aspects of this project:

- **Azky Sarafath**: Battery/Fuel level monitoring.
- **Abdul Kaiyoom**: GPS module and LoRaWAN communication.
- **Hanan Siddeek**: Accelerometer/Gyroscope for movement simulation.
- **Nazar Musthaq**: RFID/NFC tagging.

## Future Improvements

- **Predictive Maintenance**: Implement AI/ML for predicting equipment failure based on usage patterns.
- **Real GPS Integration**: Integrate a real GPS module for actual location tracking.
- **Expanded Communication**: Improve LoRaWAN implementation for remote communication.
- **Mobile App**: Develop a mobile app to enhance user experience.


---
