# Smart LPG Gas Cylinder Monitoring and Booking Alert System

An IoT-based real-time LPG monitoring system designed to continuously measure cylinder weight, estimate remaining gas percentage, detect abnormal weight drops, and generate automatic alerts.

The system integrates ESP32, HX711 load cell amplifier, and ThingSpeak cloud platform for remote monitoring and smart household energy management.

---

## Project Overview

This system measures the weight of an LPG cylinder using a load cell sensor. The readings are processed by an ESP32 microcontroller to calculate the remaining gas percentage.

When the gas level falls below a predefined threshold (20%), the system generates a low-gas alert using a buzzer and cloud notification.

Additionally, sudden abnormal weight drops are detected to identify potential gas leaks. Real-time data is uploaded to the cloud via Wi-Fi, enabling remote monitoring and improved safety.

---

## System Architecture

Load Cell → HX711 Amplifier → ESP32 → WiFi → ThingSpeak Cloud → Alert System

---

## Technology Stack

### Hardware
- ESP32 Microcontroller
- Load Cell Sensor
- HX711 Amplifier Module
- Buzzer Module

### Software
- Arduino IDE
- Embedded C/C++
- ThingSpeak Cloud Platform
- WiFi Communication

---

## Key Features

- Real-time LPG level monitoring
- Gas percentage calculation
- Low gas alert (below 20%)
- Gas leak detection using sudden weight drop logic
- Cloud-based remote monitoring
- Remote tare weight update from cloud
- Persistent storage using ESP32 Preferences
- Buzzer alert system

---

## Cloud Integration

The system uses ThingSpeak for:

- Live data visualization
- Alert code monitoring
- Remote tare weight configuration
- Cloud-based analytics

### Data Fields

- Field 1: Total Cylinder Weight
- Field 2: Gas Percentage
- Field 3: Alert Code
- Field 4: Tare Weight

---

## Security Notice

WiFi credentials and API keys have been removed from the public repository for security purposes.

Replace the placeholders in the code before running:

```cpp
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";
const char* writeAPIKey = "YOUR_API_KEY";

---

## System Design
<p align="center">
  <img src="images/system_design.jpeg" width="700"/>
</p>

## Hardware Setup
<p align="center">
  <img src="images/hardware_setup.jpeg" width="600"/>
</p>

## Alert Notification
<p align="center">
  <img src="images/lpg_alert.jpeg" width="350"/>
</p>
