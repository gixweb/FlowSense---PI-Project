# FlowSense - PI-Project
[![License](https://img.shields.io/badge/license-MIT-green)](LICENSE) [![Hardware](https://img.shields.io/badge/hardware-ESP32%20%7C%20Arduino-orange)](https://www.espressif.com/en/products/socs/esp32) [![Arduino](https://img.shields.io/badge/IDE-Arduino-blue)](https://arduino.org/) 

### Class: tad40
## Professors and Ucs:

Ronierison - Cloud Computing

Paulo - Consumer behavior 

Leonardo Trevas - English

Arnott Caiado - Internet of things (IOT)

Paulo Guimarães - Information Security

Arnott Caiado - Mobile

Maria Renay - Software Engineering


## Project Overview 

**FlowSense** is an affordable, IoT monitoring and control system for leisure and study rooms in Senac Pernambuco college. Developed as a final project for the **IoT and Embedded Systems** course at Senac Pernambuco college. 

Our solution utilizes an **ESP32 microcontroller** to gather sensor data, publish it to a cloud dashboard via **MQTT**, and automatically trigger the occupation levels in rooms in Senac Pernambuco college


### Key Features 

* **Real-time Monitoring:** Tracks the level of occupation in tbe rooms. 

* **Automated Alert** Activates Leds to alert the entry and de exit people on the rooms. 

* **Cloud Dashboard:** Visually track historical data and system status from anywhere using Adafruit IO / Blynk. 

* **Fail-Safe Mode:** Local offline logic continues running even if Wi-Fi connection is lost. 

 


## Hardware Components & Architecture 

Components Used: 
**Microcontroller**
- **ESP32-C3 Supermini**
**Sensors**
- 2 **ultrassonic sensors** (HC-SR04)
- sinalization **LEDs**

### System Architecture 

The ESP32 reads data from ultrassonic sensors pins, processes the entry and the exits loops locally, detecting the presence of people and lights the green one LED for access release adding 1 on occupation, and transmits the payload over **MQTT** (broker.emqx.io) protocol via Wi-Fi to the cloud broker.

Tests Phase: Created a "virtual button" (pin 4) for simulate the entry and exit people, allowing testing the occupation math code

Resolution Comunication Problems: Activating a USB CDC for reading data on computer


## Tech Stack & Software 

* **Embedded Programming:** C++, Arduino Framework, PlatformIO IDE 

* **IoT Protocols:** MQTT, HTTP 

* **Cloud Dashboard & Broker:** Azure (or Node-RED / HiveMQ) 

* **Libraries Used:**
  * `PubSubClient.h` (MQTT communication) 

  * `ESP32Servo.h` (PWM control for ESP32) 

  * `WiFi.h` (Wifi Conecction)

 ### Technicals Diagrams

<img width="1464" height="600" alt="diagrama de sequência Flowsense 1" src="https://github.com/user-attachments/assets/9323c4ea-7a5c-4824-b187-7c24e9d29660" />

### Firmware Fluxogram

```
[ Start / Boot ESP32 ]
           │
           ▼
┌───► [ Main Loop ]
│          │
│          ▼
│     [ Read First Ultrasonic Sensor ]
│          │
│          ▼
│     [ Read Second Ultrasonic Sensor ]
│          │
│          ▼
│     /      Has someone passed      \
│    <          through the door?     >
│     \         Which direction?     /
│                    │
│        Left (Exit) ┌────┴────┐ Right (Entry)
│                    │         │
│                    ▼         ▼
│         [ EXIT ACTION ]     [ ENTRY ACTION ]
│         • Turn on Red LED   • Turn on Green LED
│         • Count -1 in app   • Count +1 in app
│                    │         │
└────────────────────┴─────────┘

```


## Getting Started (Local Setup) 




## Midia

### Thinkercad Simulation
<img width="928" height="605" alt="image" src="https://github.com/user-attachments/assets/1c0908ca-3a3a-437c-9d9f-ca8d7bcb8be5" />


## Authors (Team Members) 

Alanna Santos - Hardware & Wiring Specialist - GitHub 

Arthur Félix - Hardware & Wiring Specialist - GitHub 

César Kauan - Embedded Software & MQTT Config - GitHub 

Giselly Rocha - Cloud Dashboard & Documentation - GitHub

Jhonata Teles - Embedded Software & MQTT Config - GitHub 

Robson Barreto - Hardware & Wiring Specialist - GitHub 

Project Mentor: Arnott Caiado
