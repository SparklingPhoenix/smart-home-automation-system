# Smart Home Automation System

## Overview

This project is a custom smart home automation system designed and deployed in a real residential apartment.

Unlike many retrofit solutions, the system was planned and integrated during the design stage of the apartment's engineering infrastructure. This approach allowed all automation, monitoring, and control components to be incorporated directly into the electrical and communication architecture from the beginning.

The project started as a personal initiative to learn low-level automation systems, embedded development, and IoT technologies using affordable and widely available hardware platforms. Over time, it evolved into a fully operational smart home system that has been continuously used and improved in a real-world environment.

### Main Control Cabinet

![Main Control Cabinet](images/power-and-control-cabinet.jpg)

## Design Principles

Several key principles guided the development of the system:

* **Scalability** – sufficient hardware and software resources for future expansion and feature development.
* **Reliability** – fault-tolerant architecture with maintainable and replaceable components.
* **Simplicity** – avoiding unnecessary complexity while meeting all functional requirements.
* **Standardization** – preference for commonly available hardware to simplify maintenance and replacement.

## Main Features

* Lighting control throughout the apartment.
* Ventilation control.
* Intrusion detection and security notifications.
* Heating automation using a gas boiler.
* Air conditioning control through infrared commands.
* Electrical network monitoring and energy auditing.
* Remote monitoring and control via web interface and Telegram.
* Event-driven automation scenarios and cross-system interactions.

## System Architecture

The system follows a centralized architecture with wired connections to all sensors and actuators.

All field devices are connected to a central control cabinet. Wireless sensors and battery-powered devices were intentionally avoided to maximize reliability, simplify maintenance, and eliminate dependency on batteries or radio communication quality.

The infrastructure was designed to allow future cable additions and modifications without major reconstruction work.

## Functional Architecture

The system follows a two-layer architecture. The Arduino-based controller handles real-time interaction with sensors and actuators, processes input signals, and executes automation logic. Operational data is then transmitted to the Orange Pi server, which provides data storage, higher-level services, and user access through a web interface and Telegram bot.

![Functional Architecture](images/Functional-architecture.png)

## Technology Stack

### Hardware

* Arduino Mega 2560 Pro (version Mini)
* Orange Pi Zero 2
* Industrial GSM module (NEOWAY M590)
* ADUM1201 isolated TTL level shifter
* ACS712 current sensors
* ZMPT101B voltage sensors
* DHT22 temperature and humidity sensors
* AHT10 temperature and humidity sensors
* Reed switches
* Mechanical push-button wall switches
* Electromechanical relay modules
* Solid-state relays

### Software

* Arduino Framework (C/C++)
* Python
* PHP
* HTML
* CSS
* JavaScript
* MariaDB
* Apache2
* Debian Linux

### Development Tools

* Arduino IDE
* Visual Studio Code
* SSH-based remote administration

## Project Status

The system is fully operational and has been running in a real residential environment for several years while continuously receiving upgrades and new functionality.
