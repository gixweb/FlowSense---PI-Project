# FlowSense - PI-Project
[![License](https://img.shields.io/badge/license-MIT-green)](LICENSE) [![Hardware](https://img.shields.io/badge/hardware-ESP32%20%7C%20Arduino-orange)](https://www.espressif.com/en/products/socs/esp32) [![Arduino](https://img.shields.io/badge/IDE-Arduino-blue)](https://arduino.org/) 

[Requirements documentation](https://senacpernambuco-my.sharepoint.com/:w:/g/personal/giselly_lima_edu_pe_senac_br/IQB-sLr0Vw7fS4TV_rmTI7p0ASHDwf-_ZBK5nmNm988gOq4?e=0D9xDY)

### Class: tad40
## Professors and Ucs:

| Professor | UC | where it is evidenced in the project |
| --- | --- | --- |
| Ronierison Maciel | Cloud Computing | APP Conection with Azure |
| Paulo Guimarães | Consumer behavior | Market research |
| Leonardo Trevas | English | This document in English |
| Paulo Pimentel | Information Security | Data Security with Cloud |
| Arnott Caiado | IOT & Mobile | IOT Material and Mobile First |
| Maria Renay | Software Engineering & QA | API development and applied testing |


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

* **Embedded Programming:** C++, Arduino Framework, PlatformIO IDE, VSCode, NodeJS

* **IoT Protocols:** MQTT, HTTP 

* **Cloud Dashboard & Broker:** Azure (or Node-RED / HiveMQ) 

* **Libraries Used:**
  * `PubSubClient.h` (MQTT communication) 

  * `ESP32Servo.h` (PWM control for ESP32) 

  * `WiFi.h` (Wifi Conecction)

## Getting Started (Local Setup) 

Follow these instructions to replicate our hardware project locally. 

The project is structured into two main layers, utilizing the following libraries and technologies:


### 1. Prerequisites & Software Installation 

- Install [VS Code](https://code.visualstudio.com/)
- Install [NodeJS](https://nodejs.org/en/download)
- Install [ArduinoIDE](https://support.arduino.cc/hc/en-us/articles/360019833020-Download-and-install-Arduino-IDE)

### 2. The Layers

#### **Firmware Layer (C++ / Arduino IDE)**
* **`WiFi.h`**: A native library used to establish the connection between the ESP32 and the local wireless network.
* **`PubSubClient.h`**: A core client library used to enable MQTT communication, allowing the microcontroller to publish occupancy count data to a specific topic (`senac/flowsense/ocupacao`).

#### **Software / Backend Layer (Node.js / VS Code)**
* **`mqtt`**: Allows the Node.js server to act as an MQTT client for Mosquitto, actively listening to incoming data published by the ESP32 board.
* **`express`**: A minimalist web framework used to spin up the API and manage application routes.
* **`socket.io`**: Enables a bidirectional, real-time communication channel with the frontend, ensuring the occupancy numbers update instantly on the user's screen without requiring a page refresh.

#### **Messaging Infrastructure**
* **Eclipse Mosquitto**: Used as the central MQTT Broker to handle data transmission between the hardware and backend.

### 3. Configuration (`src/config.h`)
Duplicate the `src/config.example.h` file, rename it to `src/config.h`, and fill in your network and MQTT credentials: 

```cpp 
// Wi-Fi Configuration 
#define WIFI_SSID "Your_WiFi_Name" 
#define WIFI_PASS "Your_WiFi_Password" 

// MQTT Broker Configuration (Example using a local/private Mosquitto broker) 
#define MQTT_SERVER "YOUR_BROKER_IP_OR_HOST" // e.g., "192.168.1.50" or "localhost"
#define MQTT_PORT 1883 
#define MQTT_USER "your_mqtt_username"       // Leave empty if no authentication is required
#define MQTT_KEY "your_mqtt_password"

```

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

### Eletric Scheme
<img width="913" height="554" alt="image" src="https://github.com/user-attachments/assets/e9517153-c7e8-4d99-956c-88792f4018ca" />



## Midia

### Thinkercad Simulation
<img width="797" height="692" alt="image" src="https://github.com/user-attachments/assets/ab2b11c4-85b9-4534-a5b9-e7df508f30ff" />

### Entry Registrs on Serial Monitor in Arduino IDE
<img width="630" height="205" alt="image" src="https://github.com/user-attachments/assets/23929b05-9d59-4816-bd89-e418bccc740a" />

### Exit Registers on Serial Monitor in Arduino IDE
<img width="625" height="201" alt="image2" src="https://github.com/user-attachments/assets/2980a7e3-b0ed-41dc-a2fe-065bca7d0907" />

### Server Conection
<img width="900" height="365" alt="WhatsApp Image 2026-06-06 at 20 03 00" src="https://github.com/user-attachments/assets/6524b2f0-ff5b-43f3-a66d-6971d2a6f387" />


### Entry and Exit in Arduino IDE
<img width="1600" height="900" alt="WhatsApp Image 2026-06-06 at 20 08 01" src="https://github.com/user-attachments/assets/16196390-a745-412f-b759-3a3819aa8faa" />


### Results
<img width="900" height="276" alt="WhatsApp Image 2026-06-06 at 20 03 00 (2)" src="https://github.com/user-attachments/assets/cf7aea48-966a-476b-b65c-7567c4af66f2" />
<img width="900" height="345" alt="WhatsApp Image 2026-06-06 at 20 02 59" src="https://github.com/user-attachments/assets/3755971e-7a98-4a85-860a-706782043057" />


## Authors (Team Members) 

Alanna Santos - Software & Front-end - GitHub 

Arthur Félix - Hardware & Wiring Specialist - GitHub 

César Kauan - Embedded Software & MQTT Config - GitHub 

Giselly Rocha - Cloud Dashboard & Documentation - GitHub

Jhonata Teles - Hardware & Wiring Specialist - GitHub 

Robson Barreto - Embedded Software & MQTT Config - GitHub 

Project Mentor: Arnott Caiado
