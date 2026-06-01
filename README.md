# FlowSense - PI-Project
[![License](https://img.shields.io/badge/license-MIT-green)](LICENSE) [![Hardware](https://img.shields.io/badge/hardware-ESP32%20%7C%20Arduino-orange)](https://www.espressif.com/en/products/socs/esp32) [![Arduino](https://img.shields.io/badge/IDE-Arduino-blue)](https://arduino.org/) 


FlowSense is an affordable, IoT monitoring and control system for leisure and study Senac Pernambuco college. Developed as a final project for the **IoT and Embedded Systems** course at Senac Pernambuco college. 

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

Estruturação do Hardware Base: Montamos o cérebro do projeto usando o ESP32-C3 SuperMini integrado a um sensor ultrassônico (HC-SR04) e um LED de sinalização na protoboard, corrigindo o fluxo de energia nas colunas da placa.

Resolução de Gargalos de Comunicação: * Ativamos o USB CDC para permitir a leitura de dados no computador.

Ajustamos a limitação de hardware do ESP32 conectando-o exclusivamente a uma rede Wi-Fi de 2.4 GHz.

Trocamos o servidor MQTT público inicial por um mais robusto (broker.emqx.io) para acabar com as falhas de conexão (erro rc=-2).

Lógica de Entrada Validada: Configuramos o sensor atual para atuar como o registro de entrada. Ele detecta a presença, acende o LED verde de liberação, soma +1 na ocupação e publica essa informação em tempo real na nuvem.

Simulador de Testes: Criamos um "botão virtual" (usando o pino 4) para simular a saída de pessoas, permitindo testar a matemática de ocupação do código sem precisar do segundo sensor agora.


## 🛠️ Hardware Components & Architecture 



## 💻 Tech Stack & Software 



## ⚙️ Getting Started (Local Setup) 



## 👥 Authors (Team Members) 

Alanna Santos - Hardware & Wiring Specialist - GitHub 

Arthur Félix - Hardware & Wiring Specialist - GitHub 

César Kauan - Embedded Software & MQTT Config - GitHub 

Giselly Rocha - Cloud Dashboard & Documentation - GitHub

Jhonata Teles - Embedded Software & MQTT Config - GitHub 

Robson Barreto - Hardware & Wiring Specialist - GitHub 

Project Mentor: Arnott Caiado
