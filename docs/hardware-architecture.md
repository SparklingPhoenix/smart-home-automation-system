# Hardware Architecture

## Control Cabinet Structure

The control cabinet is organized into several functional layers:

## Low-Voltage Control Layer

Contains the 5 V power supply, controllers, communication modules, level-shifting circuitry, and sensor power distribution.

## Power Distribution Layer

Contains the dedicated 12 V power supply used for LED lighting circuits.

## Switching Layer

Contains electromechanical relay modules and solid-state relays used for switching DC and AC loads.

## Energy Monitoring Layer

Contains voltage and current measurement devices used for diagnostics, protection, and energy monitoring.

## Hardware Components
<p align="center">
  <img src="/images/start-of-instalation-1.jpg">
  <img src="/images/control-cabinet-overview.jpg">
  <img src="/images/controller-stack.jpg"> 
</p>

The following sections describe the major hardware components used in the system.

### Arduino Mega Pro Mini (ATmega2560)

<p align="center">
  <img src="/images/Arduino-Mega-Pro-(Mini)-1.jpg" height="100">
  <img src="/images/Arduino-Mega-Pro-(Mini)-2.jpg" height="100">
  <img src="/images/Arduino-Mega-Pro-(Mini)-3.jpg" height="100">
</p>

#### Role in the System
The Arduino Mega serves as the low-level real-time controller of the system.

Its responsibilities include:
* Continuous sensor monitoring
* Processing of push buttons and reed switches
* Execution of automation scenarios
* Relay control
* Infrared transmission for air-conditioner control
* UART communication with external devices
 
#### Why It Was Selected
* Large number of GPIO pins
* Multiple hardware UART interfaces
* Native 5 V logic compatibility
* Mature development ecosystem
* Proven long-term reliability

#### Key Specifications
* ATmega2560 microcontroller
* 16 MHz clock frequency
* 256 KB Flash memory
* 54 digital I/O pins
* 16 analog inputs
* 4 hardware UART interfaces


### Orange Pi Zero 2

<p align="center">
  <img src="/images/Orange-Pi-Zero-2(1).png" width="200">
  <img src="/images/Orange-Pi-Zero-2(2).png" width="200">
  <img src="/images/Orange-Pi-Zero-2(3).png" width="200">
</p>

####  Role in the System
The Orange Pi Zero 2 provides network connectivity, data storage, and user interaction services.

Its responsibilities include:
* Web server hosting
* Telegram bot operation
* Database management
* System state storage
* Communication with the Arduino controller

####  Operating System
* Debian 11 (Bullseye)

#### Software Stack
* Apache2
* PHP
* Python
* MariaDB

#### Why It Was Selected
* Linux-based environment
* Sufficient computational resources
* Built-in networking capabilities
* Support for modern web technologies


### ADUM1201 Isolated UART Level Shifter
<p align="center">
  <img src="/images/ADUM1201-1.jpg" height="150">
  <img src="/images/ADUM1201-2.jpg" height="150">
  <img src="/images/ADUM1201-3.jpg" height="150">
</p>

####  Role in the System
* Provides galvanic isolation and level translation between the 5 V Arduino controller and the 3.3 V Orange Pi UART interface.
* Improves protection against electrical noise and voltage transients.

#### Why It Was Selected
* High communication speed
* Minimal signal latency
* Reliable galvanic isolation
* Wide operating voltage range

#### Key Specifications
* ADUM1201 isolation chip
* 2 independent communication channels
* Logic supply voltage: 2.7–5.5 V (per side)
* Isolation voltage: up to 2500 V
* Data rate: up to 25 Mbps


### NEOWAY M590 GSM Module

<p align="center">
  <img src="/images/Neoway-M590.png" height="200">
</p>

#### Role in the System

Provides alarm notifications through voice calls and SMS messages.

The module can also disable security mode when an incoming call is received from a predefined master phone number.

Following the introduction of the web interface and Telegram integration, the GSM module became largely redundant and is currently retained for legacy compatibility.

#### Why It Was Selected
* Industrial-grade design
* Simple UART integration
* AT-command interface
* Reliable operation in GSM networks

#### Key Specifications
* GSM 900 / 1800 MHz
* UART communication interface
* AT-command control
* Operating voltage: 3.3–4.5 V
* 5 V tolerant when interfaced with Arduino


## Environmental Sensors

### DHT22 Temperature and Humidity Sensors
<p align="center">
  <img src="/images/DHT-22.jpg" width="250">
</p>

#### Role in the System
DHT22 sensors are installed indoors and outdoors for environmental monitoring and climate-control automation.

#### Why They Were Selected
* Widely available and inexpensive
* Mature Arduino library support
* Large user community

#### Key Specifications
* Supply voltage: 3–6 V
* Temperature resolution: 0.1 °C
* Humidity resolution: 0.1 %
* Single-wire communication interface


### AHT10 Temperature and Humidity Sensor
<p align="center">
  <img src="/images/aht10-1.jpg" height="200">
  <img src="/images/aht10-2.jpg" height="200">
</p>

#### Role in the System
The AHT10 sensor monitors the temperature of the 12 V power supply unit and provides thermal protection functionality.

#### Why It Was Selected
* Compact design
* I²C interface
* Good measurement accuracy

#### Key Specifications
* I²C communication interface
* Operating voltage: 1.8–6.0 V
* Temperature accuracy: ±0.3 °C
* Humidity accuracy: ±2 %


## Electrical Monitoring Sensors

### ZMPT101B Voltage Sensor

<p align="center">
  <img src="/images/ZMPT101B.jpg" width="250">
</p>

#### Role in the System
Used for AC mains voltage monitoring and power-quality analysis.

#### Why It Was Selected
* Galvanic isolation
* Wide measurement range
* Low cost and availability

#### Key Specifications
* Measurement range: 0–1000 V AC
* Isolation voltage: 4000 V
* Supply voltage: 4–12 V


### ACS712 Current Sensor

<p align="center">
  <img src="/images/ACS712.jpg" width="250">
</p>

#### Role in the System
Used for current measurement in both the AC mains circuit and the 12 V power system.

The collected data is used for energy monitoring and overload detection.

#### Why It Was Selected
* Hall-effect measurement principle
* Galvanic isolation
* Widely adopted in Arduino projects

#### Key Specifications
* Measurement ranges: 5 A, 20 A, 30 A
* Supply voltage: 5 V
* Bandwidth: up to 80 kHz


### Relay Modules (SK1610 with APAN3105 / PA1A-5V Relays)

<p align="center">
  <img src="/images/relay-module-control-board-16-channel.jpg" width="500">
</p>

#### KRole in the System
Used for switching:
* Lighting circuits
* Gas boiler control
* Audible alarm siren
* Doorbell circuits
* Various low-voltage and mains-powered loads

#### Why They Were Selected
* Compact size
* Easy replacement of relay cartridges
* Good electrical compatibility with Arduino outputs
* Minimal load on controller GPIO pins

#### Key Specifications
* Coil voltage: 5 V
* Coil current: approximately 7.5 mA
* Contact configuration: SPST-NO (1 Form A)
* Contact rating: 5 A @ 250 VAC
* Contact rating: 5 A @ 30 VDC


### GEYA GSR2-1-40A Solid-State Relay (Zero-Cross)
<p align="center">
  <img src="/images/GSR2-1-40A.png" height="300">
</p>

#### Role in the System
Used for switching inductive AC loads, specifically the forced ventilation system.

#### Why It Was Selected
* Zero-cross switching
* No contact arcing
* Reduced electrical interference
* Improved reliability when switching motors

#### Key Specifications
* DC-to-AC solid-state relay
* Load current: 40 A
* Control voltage: 3–32 VDC
* Load voltage: 24–480 VAC
* Integrated TVS protection


### Push Buttons and Reed Switches

#### Role in the System
Momentary push buttons are used as the primary local user interface for lighting and automation control.

Reed switches are used for door and window position monitoring as part of the security and climate-control subsystems.

#### Why They Were Selected
* Simple and reliable operation
* Long service life
* Easy integration with low-voltage control circuits
* Support for multi-location switching configurations


### Infrared LED Transmitter

<p align="center">
  <img src="/images/IR-LED.jpg" width="200">
</p>

#### Role in the System
Used for air-conditioner control by transmitting infrared commands that emulate the original remote control.

#### Why It Was Selected
* Low cost
* Simple implementation
* Full compatibility with existing HVAC equipment


### Hardware Summary

The hardware platform combines a deterministic microcontroller-based automation layer with a Linux-based service layer.

This architecture provides reliable real-time control while supporting modern user interfaces, remote access, data storage, and future expansion capabilities.

The system has been operating continuously in a real residential environment for several years, demonstrating long-term stability, maintainability, and practical reliability.

