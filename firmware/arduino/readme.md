# Originally developed as part of the Smart Home Automation Platform.

## Overview

This repository contains a collection of production-proven modules from my Embedded Home Automation Platform project.

Unlike demonstration projects, these modules have been continuously operating in a real apartment for several years, undergoing numerous improvements based on practical эксплуатация rather than laboratory testing.

The system combines:
* autonomous microcontroller firmware;
* Linux-based server software;
* web interface;
* database synchronization;
* Telegram notifications;
* GSM communication;
* security subsystem;
* climate control;
* energy monitoring;
* intelligent lighting scenarios.

The project follows a distributed architecture where each controller performs only deterministic real-time tasks while high-level services are executed on a Linux server.

---

# Architecture

The project consists of several independent layers.

```
                Web Interface
                     │
                     ▼
             MariaDB Database
                     │
                     ▼
               Orange Pi Server
          (PHP + Python services)
                     │
         UART communication channel
                     │
                     ▼
              Arduino Mega 2560
        (real-time control subsystem)
                     │
 ┌───────────────┬───────────────
 │               │               │
Sensors      Relays         GSM modem
 │               │               │
Temperature   Lighting      Voice calls
Humidity      Heating       USSD
Doors         Ventilation   SMS
Power         Security
```

---

# Design Philosophy

The firmware was written according to several engineering principles.

## Event-driven architecture

The controller never executes long sequential scripts.

Every subsystem behaves as an independent finite-state machine.

This allows several completely unrelated processes to execute "simultaneously":

* security monitoring;
* climate regulation;
* GSM communication;
* power monitoring;
* lighting automation;
* web synchronization.

---

## Non-blocking timing

Almost every long operation is implemented through `millis()` timers.

Instead of waiting,

```
delay(30000);
```

the firmware stores timestamps:

```
Start = millis();

if (millis() - Start >= Timeout)
{
    ...
}
```

This allows dozens of independent timers to run simultaneously.

---

## State Machine Architecture

Many subsystems operate through state variables.

Example:

```
0 → disabled

1 → checking modem

2 → restarting modem

3 → configuring modem

4 → normal operation
```

Each state performs only its own responsibility.

---

## Modular Design

Each subsystem is isolated into a dedicated function.

Examples include:
* GSM modem control
* Security system
* Power monitoring
* Temperature acquisition
* Ventilation
* Lighting
* Telegram integration
* Database synchronization

Modules interact only through shared state variables.

---

# Main Functional Blocks

## Security System

The apartment security subsystem supports:
* automatic arming;
* automatic disarming;
* intrusion detection;
* alarm mode;
* GSM notification;
* Telegram notification;
* audible siren;
* flashing light alarm;
* EEPROM state restoration.

Door lock position and door position are processed independently.

A scenario engine recognizes valid user actions while detecting unauthorized access.

---

## GSM Communication

The GSM subsystem includes:
* modem initialization;
* automatic recovery after failures;
* network registration monitoring;
* signal quality monitoring;
* voice calls;
* USSD requests;
* incoming call processing;
* remote security control.

The modem continuously verifies its own operational state and performs automatic recovery if necessary.

---

## Climate Control

The climate subsystem manages:
* gas boiler;
* air conditioner;
* ventilation.

Temperature is measured from multiple sensors.

Humidity is also monitored.

The control logic supports several operating modes:
* normal;
* economy;
* automatic switching;
* power-loss scenarios.

---

## Power Monitoring

Electrical parameters are continuously measured.

The firmware calculates:
* RMS voltage;
* RMS current;
* DC current;
* AC power;
* DC power.

The system automatically reacts to:
* mains power loss;
* power restoration;
* power supply overload;
* overheating.

---

## Lighting Automation

Lighting scenarios include:
* hallway assistance;
* alarm flashing;
* blackout mode;
* local lighting;
* room presets;
* doorbell indication.

Lighting states are restored automatically after temporary warning scenarios.

---

## Telegram Integration

Important events are immediately delivered to Telegram.

Examples include:
* security armed;
* security disarmed;
* alarm;
* doorbell;
* mains power loss;
* mains power restoration;
* configuration changes.

The controller simply launches the required PHP script on the Linux server.

---

## Database Synchronization

Sensor values and configuration are continuously synchronized with MariaDB.

Instead of implementing a database client on the microcontroller, Arduino transmits SQL commands through UART.

The Orange Pi executes them directly.

This approach keeps the firmware lightweight while providing complete database integration.

---

# Reliability

The firmware contains multiple recovery mechanisms.

Examples include:
* automatic GSM modem restart;
* DHT22 watchdog;
* EEPROM configuration storage;
* recovery after reboot;
* overload protection;
* thermal shutdown;
* communication verification.

The system was designed to recover automatically from most common hardware failures without user intervention.

---

# Technologies

### Hardware
* Arduino Mega 2560
* Orange Pi
* Neoway M590 GSM modem
* DHT22
* AHT20
* ACS current sensors
* ZMPT voltage sensor
* Relay modules

---

### Software
* Arduino C++
* PHP
* Python
* JavaScript
* HTML
* CSS
* MariaDB
* Linux

---

# Engineering Features

Compared to many hobby Smart Home projects, this system emphasizes:
* deterministic behaviour;
* real-time processing;
* modular architecture;
* fault tolerance;
* production operation;
* maintainability;
* distributed execution.

Every subsystem has been repeatedly refined through long-term practical operation.

---

# Current Status

The system has been operating continuously in a real residential environment for several years.

Many algorithms have evolved through practical experience rather than theoretical design, resulting in numerous optimizations and reliability improvements.

Development continues as new automation scenarios and hardware modules are introduced.
