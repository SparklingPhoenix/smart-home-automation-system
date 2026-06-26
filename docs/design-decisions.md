# Design Decisions

## Engineering Philosophy

The primary objective of this project was not to build the most feature-rich smart home system, but to develop a platform that is reliable, maintainable, and capable of operating continuously for many years with minimal maintenance.

Whenever multiple technical solutions were available, preference was given to those that improved long-term reliability, simplified maintenance, or increased future expandability.

The following sections describe the most important engineering decisions made during the design process.

---

# Reliability Decisions

## Fully Wired Infrastructure

All sensors and control devices use wired connections.

Wireless sensors were intentionally avoided for several reasons:

* No batteries requiring periodic replacement
* Immunity to radio interference
* Stable communication
* Long-term reliability
* Predictable response time

Although installation requires more cabling during construction, the result is a considerably more robust automation platform.

---

## Centralized Control Cabinet

All field wiring terminates in a single centralized control cabinet.

Instead of using conventional hidden junction boxes, dedicated cable routes were installed during apartment construction.

The cable infrastructure consists of:

* DKC skirting trunking with integrated outlets and switches
* 50 mm conduit installed above the suspended ceiling

This approach allows additional cables to be installed at any time without damaging finished walls or ceilings.

The centralized architecture also greatly simplifies maintenance and troubleshooting.

---

## Why Arduino Mega?

The Arduino Mega was selected as the real-time controller for several practical reasons.

Unlike Linux-based single-board computers, the Arduino provides deterministic real-time behavior and directly interfaces with field devices.

Additional advantages include:

* Native 5 V logic
* Four hardware UART interfaces
* Large number of GPIO pins
* Mature software ecosystem

An important practical consideration is the output current capability.

Each Arduino Mega GPIO pin can source up to approximately 40 mA, allowing direct control of low-power relay coils without additional driver circuitry.

The integrated pull-up resistors also permit direct connection of mechanical push buttons using cable lengths of up to approximately 10 meters while maintaining reliable operation.

In comparison, the Orange Pi Zero 2 GPIO pins provide significantly lower output current, making them less suitable for directly driving electromechanical loads.

These characteristics ultimately led to separating the system into:

* a deterministic low-level controller (Arduino),
* and a Linux-based service layer (Orange Pi).

---

## Solid-State Relay for Inductive Loads

Forced ventilation is switched using a zero-cross solid-state relay rather than a conventional mechanical relay.

During early testing it became evident that disconnecting an inductive motor load with a mechanical relay generated significant electrical noise.

In several cases this interference caused unexpected Arduino resets.

The final solution consisted of:

* a high-quality regulated 5 V power supply,
* zero-cross switching,
* and a generously rated 40 A SSR.

Although the relay is oversized for the actual motor current, the additional safety margin improves long-term reliability and thermal performance.

---

## Removable Interface Relays

Lighting and auxiliary outputs are switched using removable interface relays.

This solution offers several practical advantages:

* Compact installation
* Fast relay replacement
* Easy maintenance
* Low GPIO loading

Practical experience showed that these relays operate reliably with resistive loads up to approximately 3 A.

However, many modern LED luminaires generate high inrush currents because of their internal capacitor power supplies.

For these loads, additional inrush-current limiting circuitry (LC or Pi filters) is recommended to maximize relay lifetime.

---

## Low-Voltage Push Buttons

All wall switches are implemented as momentary push buttons operating at 5 V.

The switches are therefore electrically isolated from both the 12 V and 230 VAC power circuits.

This design offers several advantages:

* Improved user safety
* Reduced wiring complexity
* Unlimited parallel push-button installation
* Simple implementation of virtual multi-way switching

The control logic is implemented entirely in software.

---

## Soldered Connections

All controller interconnections inside the cabinet are soldered.

Connectors were intentionally minimized wherever practical.

Although connectors simplify assembly, they also introduce additional potential failure points caused by:

* Vibration
* Surface oxidation
* Contact resistance
* Mechanical wear

For a permanently installed residential system, soldered connections provide greater long-term reliability.

---

# Lessons Learned

Several years of continuous operation provided valuable practical experience.

Some original design decisions would likely be implemented differently if the project were started today.

---

## Orange Pi vs ESP32

The Orange Pi Zero 2 has proven to be reliable and provides a comfortable Linux environment.

However, practical experience suggests that its computational resources are significantly underutilized.

Today, an ESP32 would likely be selected instead.

Potential advantages include:

* Lower power consumption
* Simpler firmware backup and recovery
* Faster startup
* Reduced heat generation
* No cooling fan required
* Higher overall system reliability

---

## GSM Module

The project originally relied exclusively on Arduino.

At that stage, GSM communication represented the only practical method for remote notifications.

After introducing the Orange Pi, web interface, and Telegram bot, the GSM subsystem became largely redundant.

If the project were redesigned today, the GSM modem would probably be omitted.

---

## Mechanical Push Buttons

Conventional mechanical wall switches are designed primarily for switching mains voltage.

After several years of operation, slight oxidation of the contact surfaces increased the contact resistance.

While insignificant at 230 VAC, this effect became noticeable in the 5 V control circuit.

Occasional maintenance of the switch contacts became necessary.

For a future revision of the project, capacitive touch switches would likely replace conventional mechanical push buttons.

---

# Future Development

Although the system has been operating successfully for several years, the architecture was intentionally designed with sufficient hardware and software reserves for future expansion.

Possible future improvements include:

* Migration from Orange Pi to ESP32
* Additional environmental sensors
* Enhanced energy analytics
* Extended automation scenarios
* Improved user interface
* Integration with additional smart-home protocols

