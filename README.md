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


## 📋 Project Overview 

**FlowSense** is an affordable, IoT monitoring and control system for leisure and study rooms in Senac Pernambuco college. Developed as a final project for the **IoT and Embedded Systems** course at Senac Pernambuco college. 

Our solution utilizes an **ESP32 microcontroller** to gather sensor data, publish it to a cloud dashboard via **MQTT**, and automatically trigger the occupation levels in rooms in Senac Pernambuco college


### Key Features 

* **Real-time Monitoring:** Tracks the level of occupation in tbe rooms. 

* **Automated Alert** Activates Leds to alert the entry and de exit people on the rooms. 

* **Cloud Dashboard:** Visually track historical data and system status from anywhere using Adafruit IO / Blynk. 

* **Fail-Safe Mode:** Local offline logic continues running even if Wi-Fi connection is lost. 

 


## 🛠️ Hardware Components & Architecture 

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


## 💻 Tech Stack & Software 

* **Embedded Programming:** C++, Arduino Framework, PlatformIO IDE 

* **IoT Protocols:** MQTT, HTTP 

* **Cloud Dashboard & Broker:** Azure (or Node-RED / HiveMQ) 

* **Libraries Used:**
  * `PubSubClient.h` (MQTT communication) 

  * `ESP32Servo.h` (PWM control for ESP32) 

  * `WiFi.h` (Wifi Conecction)
  

## ⚙️ Getting Started (Local Setup) 




## 👥 Authors (Team Members) 

Alanna Santos - Hardware & Wiring Specialist - GitHub 

Arthur Félix - Hardware & Wiring Specialist - GitHub 

César Kauan - Embedded Software & MQTT Config - GitHub 

Giselly Rocha - Cloud Dashboard & Documentation - GitHub

Jhonata Teles - Embedded Software & MQTT Config - GitHub 

Robson Barreto - Hardware & Wiring Specialist - GitHub 

Project Mentor: Arnott Caiado
