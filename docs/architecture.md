# System Architecture

## Overview

The overall architecture of the system was determined by the combination of functional requirements and long-term reliability goals. The electrical installation follows a centralized radial topology, where all sensors, switches, actuators, and peripheral devices are connected directly to a single control cabinet.

To maximize reliability and service life, the system relies exclusively on wired sensors and devices. This approach eliminates battery maintenance requirements and reduces susceptibility to radio-frequency interference, providing predictable long-term operation.

Functionally, the system is divided into two distinct control layers. The Arduino-based low-level controller is responsible for real-time interaction with sensors and actuators, execution of automation logic, and control of relay outputs. The Orange Pi high-level controller provides data storage, network connectivity, user interaction, and visualization through a web interface and Telegram integration.

## Architecture Summary

The system follows a centralized wired architecture with a clear separation between real-time control and user-facing services.

The Arduino Mega controller performs deterministic low-level automation tasks, while the Orange Pi Zero 2 provides data storage, network connectivity, and communication interfaces for end users.

This separation allows the automation layer to remain operational independently of web services or Internet connectivity.

## Design Principles

The system was designed according to the following engineering principles:

* **Scalability** - the architecture should provide sufficient computational and hardware resources for future expansion, feature additions, and integration of new devices.
* **Reliability** – the system should maintain stable operation under long-term continuous use while remaining serviceable and fault-tolerant.
* **Simplicity** – the architecture should avoid unnecessary complexity while providing all functionality required by the project.
* **Standardization** – whenever possible, commonly available and standardized components should be used to simplify maintenance, replacement, and future upgrades.

## Control Layers

### Low-Level Controller

The low-level control layer is implemented using an Arduino Mega (Pro Mini version).

<p align="center">
  <img src="/images/Arduino-Mega-Pro-(Mini)-1.jpg" width="150">
  <img src="/images/Arduino-Mega-Pro-(Mini)-2.jpg" width="150">
  <img src="/images/Arduino-Mega-Pro-(Mini)-3.jpg" width="150">
</p>

Its primary responsibilities include:

* polling all sensors and monitoring inputs;
* processing signals from switches, push buttons, and reed contacts;
* executing automation logic and control scenarios;
* generating control signals for relay outputs;
* transmitting infrared commands for air-conditioner control;
* handling UART communication with external modules and controllers.

The Arduino controller is responsible for all time-critical operations and continues to operate independently of the network infrastructure.

### High-Level Controller

The high-level control layer is implemented using an Orange Pi Zero 2 single-board computer.

<p align="center">
  <img src="/images/Orange-Pi-Zero-2(1).png" width="150">
  <img src="/images/Orange-Pi-Zero-2(2).png" width="150">
  <img src="/images/Orange-Pi-Zero-2(3).png" width="150">
</p>

Its primary responsibilities include:

* providing a web-based user interface over the local network;
* providing remote access through a Telegram bot;
* storing current system state information and historical data;
* maintaining the MariaDB database;
* exchanging data with the Arduino controller via UART.

Unlike the Arduino controller, the Orange Pi is not involved in direct hardware control. Instead, it serves as the communication, storage, and user interaction layer of the system.

## Communication Interfaces

The system utilizes several communication channels between its components and external devices.

### UART Communication

Data exchange between the main controllers is implemented using UART interfaces.

Communication between the Arduino Mega (5 V logic) and the Orange Pi Zero 2 (3.3 V logic) is performed through an ADUM1201 isolated TTL level shifter.

The NEOWAY M590 GSM module is connected directly to the Arduino Mega via UART. Additional level shifting is not required, as the module is tolerant of 5 V logic levels.

An additional UART-connected device is present on the Arduino Mega controller; however, it belongs to a separate integrated audio system located in a neighboring control cabinet and is therefore outside the scope of this project.

### Network Connectivity

The Orange Pi Zero 2 is connected to the home network via Wi-Fi and provides access to the web interface and Telegram services.

A wired Ethernet connection would be preferable from a reliability perspective; however, the physical location of the control cabinet makes Ethernet cabling impractical in the current installation.

### Infrared Communication

An infrared LED transmitter is directed toward the air-conditioning unit at a distance of approximately 4 meters.

The Arduino controller generates modulated IR signals that emulate the original remote-control commands.

### Boiler Control Interface

The Vaillant gas boiler is controlled through a relay output configured as a galvanically isolated dry contact, allowing the system to operate as an external room thermostat.

### GSM Communication

The NEOWAY M590 industrial GSM module is connected to the regional cellular network and provides alarm notifications via voice calls and SMS messages.

## Data Flow

The following examples illustrate how information flows through the system during typical operation.

### Local Control Scenario

When a user presses a wall switch or push button:

```text
Switch pressed
    ↓
Arduino receives input signal
    ↓
Automation logic executed
    ↓
Relay output activated
    ↓
Lighting state changed
    ↓
Status transmitted to Orange Pi
    ↓
Web interface updated
    ↓
Optional Telegram notification generated
```

Telegram notifications for routine events are disabled by default and can be enabled when required.

### Remote Control Scenario

When a command is issued through the web interface or Telegram bot:

```text
User command received
    ↓
Orange Pi updates system state
    ↓
Command transmitted to Arduino
    ↓
Automation logic executed
    ↓
Relay output activated
    ↓
Lighting state changed
    ↓
Updated status returned to Orange Pi
    ↓
Web interface refreshed
```

This architecture ensures that all hardware control operations are executed exclusively by the Arduino controller, while the Orange Pi serves as the user interaction and data management layer.

