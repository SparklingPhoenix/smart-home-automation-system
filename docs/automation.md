# Automation Scenarios

## Overview

The smart home market offers a wide range of commercial automation solutions. However, one common limitation is the difficulty of integrating devices and services from different vendors into a single, unified system.

The primary motivation behind developing this custom smart home platform was to consolidate all essential residential automation functions into a single flexible and expandable system.

The main design goals were:
* Scalability and the ability to install an arbitrary number of switches and control points
* Full remote access to all system functions from a smartphone or PC
* Virtual duplication of all physical controls
* Continuous monitoring of all measured and controlled parameters
* Support for both direct and cross-system automation scenarios
* Maximum flexibility in configuration and customization
* Optimization of electrical and gas consumption
* Independence from external cloud services requiring permanent Internet connectivity

The resulting platform combines lighting control, climate management, security, energy monitoring, ventilation control, and remote user interaction within a single integrated architecture.



## Lighting Automation

### Local Lighting Control

A key feature of the lighting subsystem is the use of momentary push buttons instead of conventional latching wall switches.

All switches utilize normally-open (NO) dry contacts and are connected in parallel, allowing multiple control points to operate the same lighting zone without requiring traditional multi-way wiring schemes.

A button press triggers an address-specific automation scenario executed by the Arduino controller, which subsequently controls the corresponding relay outputs.

Each room typically contains multiple lighting groups:
* Ambient lighting (12 V LED strips)
* General lighting (220 V ceiling fixtures)

This architecture enables several lighting combinations within a single room.

### Lighting Selection Logic

When all lighting groups are off:
1. The first button press starts a configurable timer (default: 3 seconds) and activates the primary lighting group.
2. If another button press occurs before the timer expires, the system switches to the next predefined lighting configuration.
3. The timer is restarted after each selection.
4. Additional presses cycle through all available lighting combinations.
5. Once the timer expires, the current configuration is accepted and remains active.

When lighting is already active:
* A single button press turns off all previously activated lighting groups.
* The cycle then returns to its initial state.

This approach allows multiple lighting scenes to be controlled using a single physical button.

### Extended Functions

Each button also supports a secondary function activated by holding the button for more than one second.

Examples include:
* Starting or stopping the ventilation system
* Controlling lighting in adjacent rooms
* Activating predefined lighting-selection modes
* Executing apartment-wide commands

One example is the Blackout scenario, available from the switch located near the entrance door.

This scenario immediately turns off all lighting circuits throughout the apartment and is typically used when leaving the premises.

All timing parameters are configurable through the web interface.

### Remote Lighting Control

Every physical switch has a synchronized virtual counterpart available through both the web interface and Telegram.

Whenever a physical button is pressed, the corresponding virtual control immediately reflects the new state.

### Web Interface

The lighting section of the web interface consists of two modes:

#### Simplified View
Provides quick on/off control for the primary lighting group of each room.
<p align="center">
  <img src="/images/screenshot/Web-interface-switch-panel-universal-1.jpg" height="350">
  <img src="/images/screenshot/Web-interface-switch-panel-universal-2.jpg" height="350">
</p>
<video width="100%" max-width="720px" controls muted loop>
  <source src="/videos/Web-interface-switch-panel-universal.mp4" type="video/mp4">
</video>

#### Detailed View
Allows the user to select a room and individually control all available lighting groups and lighting scenes.
<p align="center">
  <img src="/images/screenshot/Web-interface-switch-panel-manual.jpg" height="200">
</p>
<video width="100%" max-width="720px" controls muted loop>
  <source src="/videos/Web-interface-switch-panel-manual.mp4" type="video/mp4">
</video>

A detailed description, screenshots, and demonstration videos are provided in web-interface.md.

#### Telegram Control

The Telegram interface largely mirrors the simplified web interface.
<p align="center">
  <img src="/images/screenshot/Telegram-bot-menu-light-switch.jpg" height="200">
</p>
<video width="100%" max-width="720px" controls muted loop>
  <source src="/videos/Telegram-bot-light-switching.mp4" type="video/mp4">
</video>

Due to Telegram platform limitations, real-time automatic state synchronization is not available.

Instead, users can request an updated system status through the bot menu.

The bot then returns the current lighting status for all rooms.

Any lighting changes performed through Telegram are immediately reflected within the web interface.

A detailed description of the Telegram implementation is provided in telegram-bot.md.

#### Control Priority

All control methods have equal priority:
* Physical buttons
* Web interface
* Telegram interface

The system automatically resolves state synchronization between all control channels.

For example, if lighting is turned on using a physical switch and later turned off through the web interface or Telegram, the corresponding Arduino scenario is automatically updated to prevent inconsistent behavior.



## Climate Control

### Heating Control

The apartment is heated by a gas boiler manufactured by Vaillant.

Integration with the boiler presented several challenges:
* The official remote-control ecosystem is relatively expensive.
* No public API is available for third-party integration.
* The internal eBus communication protocol is proprietary.

To avoid modifying the boiler's internal control electronics, integration was implemented using the standard thermostat input (RT24 dry-contact interface).

This approach provides reliable boiler start/stop control while maintaining complete electrical isolation.

#### Achieved Benefits

The integration provides:
* Remote boiler control through the web interface
* Remote boiler control through Telegram
* Detailed thermostat status information
* Integration with cross-system automation scenarios
* Reduced gas consumption

Indoor and outdoor temperatures are measured using DHT22 sensors.

The system supports configurable temperature calibration offsets ranging from -3°C to +3°C.
<p align="center">
  <img src="/images/screenshot/Web-interface-prefences-panel-2.jpg" height="200">
</p>

Calibration values can be adjusted through the web interface.

### Custom Thermostat Algorithm

Conventional dry-contact thermostats typically rely on hysteresis control.

Although modern electronic thermostats offer improved temperature regulation, they still cannot fully account for the thermal inertia of both the heating system and the building itself.

An additional challenge is that the boiler used in this project does not provide a simple isolated feedback signal indicating circulation-pump activity.

As a result, a conventional thermostat may interrupt a heating cycle prematurely, which is undesirable from both efficiency and equipment-longevity perspectives.

#### Design Objectives

The custom control algorithm was developed to address several goals:
* Prevent premature interruption of boiler heating cycles
* Account for residual heat stored within radiators
* Compensate for thermal inertia of the apartment
* Improve temperature stability around the desired setpoint

#### Control Strategy

A configurable minimum boiler runtime is enforced.

This runtime was determined experimentally by measuring the time required for the heating circuit to complete a full heating cycle.
<p align="center">
  <img src="/images/screenshot/Web-interface-prefences-panel-3.jpg" height="200">
</p>

After each completed cycle, the controller evaluates the residual thermal energy released by the radiators and adjusts the temperature threshold used to initiate the next heating cycle.

This adaptive approach continuously compensates for thermal inertia and attempts to maintain the average indoor temperature as close as possible to the user-defined setpoint.

Further implementation details are intentionally omitted.

#### Practical Results

During more than three years of continuous operation, the adaptive thermal-compensation algorithm has demonstrated significantly improved temperature stability compared to conventional consumer Wi-Fi thermostats.

Combined with the 0.1°C resolution of the DHT22 temperature sensor, the system has proven capable of reacting effectively to changing environmental conditions.

Practical observations indicate that events such as:
* Opening a window for ventilation
* Rapid changes in outdoor temperature

do not cause significant long-term deviation from the desired indoor temperature.

The adaptive control logic automatically incorporates these disturbances into subsequent heating cycles.

### Air Conditioner Control

The apartment uses a Cooper & Hunter CH-S09XN7 split-system air conditioner.

Unlike modern smart HVAC systems, this unit provides neither Wi-Fi connectivity nor a wired remote-control interface.

Control is available exclusively through an infrared remote control.

To integrate the air conditioner into the automation platform, an infrared LED transmitter connected directly to the Arduino controller is used to emulate commands from the original remote control.

#### Control Algorithm

The air-conditioning logic is based on the same thermal-management principles used by the heating subsystem, but adapted for cooling operation.

The controller includes configurable:
* Minimum runtime limits
* Minimum idle-time limits

These restrictions prevent excessive short cycling of the HVAC equipment.

#### Infrared Command Implementation

Infrared commands were captured from the original remote control using a temporary Arduino-based IR receiver setup.

One peculiarity of this HVAC system is that every transmitted IR command contains a complete snapshot of the current operating configuration rather than a simple button code.

Each stored command therefore occupies a relatively large amount of microcontroller memory.

To optimize memory usage, only four commands were implemented:
* Fan Speed 1
* Fan Speed 2
* Fan Speed 3
* Power Off
<p align="center">
  <img src="/images/screenshot/Web-interface-prefences-panel-4.jpg" height="200">
</p>

Fan speed selection is performed automatically according to the difference between the current room temperature and the desired setpoint.

As the measured temperature approaches the target value, fan speed is gradually reduced to minimize overshoot and improve comfort.

### Cross-System Climate Scenarios

Several additional automation scenarios are implemented:
* Air-conditioner startup is blocked while a window is open.
* Automatic cooling resumes after the window is closed, following a configurable delay period.
* Emergency heating mode is activated if indoor temperature drops below +6°C, preventing potential freezing of the heating * * system.

These interactions demonstrate how climate-control functions are integrated with other automation subsystems throughout the project.



## Security System

### Overview

The security subsystem was one of the primary features implemented in the project.

Its main purpose is to detect unauthorized entry into the apartment and immediately notify the owner through multiple communication channels.

The system operates independently of Internet connectivity and supports both local and remote alarm notification mechanisms.

### Armed Mode

When armed mode is enabled, the system continuously monitors all security sensors connected to the apartment.

The monitored devices include:
* Entrance door reed switches
* Window reed switches
* Additional intrusion detection sensors

The current security status is synchronized across all user interfaces:
* Web interface
* Telegram bot
* GSM notification module
<p align="center">
  <img src="/images/screenshot/Web-interface-prefences-panel-7.jpg" height="200">
  <img src="/images/screenshot/Telegram-bot-menu-security.jpg" height="200">
</p>

The user can enable or disable armed mode through any available control channel.


### Intrusion Detection

The security subsystem is based on magnetic reed switches installed on doors and windows.

Each sensor is continuously monitored by the Arduino controller.

When a protected opening changes state while the system is armed, an alarm event is generated immediately.

#### Alarm Trigger Conditions

Examples of events that trigger an alarm:
* Opening the entrance door
* Opening a window

The triggering logic is intentionally simple to maximize reliability and minimize false negatives.

### Alarm Actions

When an intrusion event is detected, the controller executes a predefined alarm sequence.

Typical actions include:

#### Audible Alarm

Activation of a 12 V siren installed inside the apartment.

The siren serves two purposes:
* Deterring unauthorized entry
* Alerting nearby occupants

#### Telegram Notification

An alarm message is sent to the owner's Telegram account.

Typical information includes:
* Event type
* Triggered sensor
* Date and time of occurrence

#### GSM Notification

A voice call is placed through the NEOWAY M590 GSM module.

This mechanism remains operational even if:
* Internet connectivity is unavailable
* Telegram services are unreachable
* The Orange Pi is offline

SMS notifications containing event details can also be sent when required.


### Disarming Methods

Several methods are available for disabling armed mode.

#### Web Interface

The user can arm or disarm the apartment through the web dashboard.

#### Telegram Bot

Security mode can be controlled remotely using Telegram commands and menus.

#### GSM Call Authentication

The GSM module supports a simple caller-ID-based disarming mechanism.

When an incoming call is received from a predefined master phone number:
1. The call is identified.
2. The caller's number is verified.
3. The alarm mode is automatically disabled.
The call is terminated without answering.

This approach allows security mode to be disabled even without Internet access.

#### Security Design Considerations

Several design decisions were made to improve reliability:
* All sensors are wired.
* No batteries are required for field devices.
* No wireless security sensors are used.
* Alarm logic is executed directly on the Arduino controller.
* Security operation does not depend on cloud services.
* Multiple notification channels are available.

This architecture minimizes the number of potential failure points while maintaining predictable operation.



## Ventilation Control

### Manual Control

The forced ventilation system offers flexible control options and can be operated both locally via mechanical push buttons and remotely via the web interface or Telegram bot.

To toggle ventilation in a specific zone (bathroom, bedroom, or dressing room), press and hold the corresponding mechanical button for 1 second.

### Automatic Control

The following automatic modes and settings are available in the web interface:
* Window interlock — prevents ventilation from starting if the window is closed
* Auto-start on window opening
* Ventilation timer — automatically starts countdown after lights are turned off in the respective room
* Timer duration adjustment



## Energy Monitoring

### Overview

The energy monitoring subsystem provides real-time visibility into the electrical parameters of both the mains power supply and internal low-voltage circuits.

Its primary goals are:
* Monitoring power quality
* Detecting abnormal operating conditions
* Protecting power supplies and connected equipment
* Providing historical operating data
* Supporting energy-efficiency analysis

The subsystem continuously collects data from voltage and current sensors connected to the Arduino controller.

### AC Mains Monitoring

The system monitors the primary electrical supply of the apartment using voltage and current sensors.

Measured parameters include:
* AC mains voltage
* Load current
* Estimated power consumption
* Supply stability

The collected information allows the user to observe long-term trends and identify abnormal operating conditions.

#### Practical Applications

The monitoring system can be used to detect:
* Voltage drops
* Voltage spikes
* Unusual load increases
* Power outages
* Equipment failures

The collected data is available through the web interface and can be used for diagnostics and troubleshooting.

### Low-Voltage System Monitoring

In addition to mains monitoring, the system supervises its own 12 V power infrastructure.

Monitored parameters include:
* Power supply voltage
* Load current
* Power supply temperature

This information is used to detect:
* Overload conditions
* Thermal stress
* Potential power supply failures

The temperature of the 12 V power supply is monitored using an AHT10 sensor mounted inside the control cabinet.

### Data Collection and Processing

Sensor data is continuously acquired by the Arduino controller and transferred to the Orange Pi through the UART interface.

The Orange Pi:
* Stores measurements in the database
* Provides historical data access
* Generates graphical representations
* Makes the information available through the web interface

This architecture separates real-time data acquisition from long-term storage and visualization.

### Data Visualization

Electrical measurements can be viewed through the web interface.

The monitoring dashboard provides:
* Current sensor values
* Historical trends
* System status information
* Diagnostic indicators

The visualization layer was designed primarily for practical troubleshooting rather than detailed energy accounting.

### Reliability Considerations

The monitoring subsystem is intended to provide operational awareness rather than certified metering functionality.

Sensor accuracy is sufficient for:
* Trend analysis
* Equipment diagnostics
* Load estimation
* Protection logic

The system should not be considered a replacement for certified utility-grade energy meters.



## Notification System

### Overview

The notification subsystem informs the user about important events occurring within the smart home platform.

Several independent communication channels are supported to improve reliability and ensure that critical alerts are delivered even if one communication path becomes unavailable.

Supported channels include:
* Telegram
* GSM voice calls
* SMS messages

### Telegram Notifications

Telegram serves as the primary notification channel.

The system can generate notifications for:
* Security alarms
* System state changes
* Climate-control events
* Power-related events
* User-defined automation events

Telegram was selected because it provides:
* Fast message delivery
* Cross-platform support
* Remote access without VPN configuration
* Simple integration through a bot API

### Notification Strategy

To avoid excessive message volume, many notifications are disabled by default.

Users can request detailed system status information on demand through the Telegram interface.

This approach significantly reduces unnecessary notifications while maintaining access to critical information.

### GSM Notifications

A GSM notification channel is implemented using a NEOWAY M590 industrial modem.

Although originally designed as the primary remote-notification mechanism, its role has changed since the introduction of the Orange Pi and Telegram integration.

Today, GSM functionality primarily serves as a backup communication channel.

#### Supported Functions
* Voice call notifications
* SMS notifications
* Remote security-mode control

#### Advantages

The GSM subsystem remains operational even when:
* Internet access is unavailable
* Telegram services are inaccessible
* Local network infrastructure has failed


### Notification Priorities

Different event types use different notification strategies.

#### Critical Events

Examples:
* Intrusion detection
* Security alarm activation

Actions:
* Telegram notification
* GSM call
* Optional SMS message
* Local siren activation

Informational Events

Examples:
* Lighting changes
* Climate-control actions
* Routine automation events

Actions:
* Logged in the system database
* Displayed through the user interface
* Optional Telegram notification

This prioritization prevents notification overload while ensuring that critical events receive immediate attention.



## Automation Engine

### Overview

The automation engine is responsible for coordinating interactions between all subsystems of the smart home platform.

It processes incoming events, evaluates automation rules, and executes the corresponding actions.

The automation logic is primarily implemented on the Arduino controller to ensure deterministic real-time operation.

### Event Processing

The system follows an event-driven architecture.

Examples of events include:
* Button presses
* Reed switch activations
* Sensor value changes
* Timer expirations
* User commands from the web interface
* User commands from Telegram

Every event is processed independently and may trigger one or more automation scenarios.

### Scenario Execution

Automation scenarios consist of predefined actions associated with specific events.

A typical execution sequence is:

Event detected
       ↓
Event validated
       ↓
Automation logic executed
       ↓
Output state updated
       ↓
System status synchronized
       ↓
User interfaces updated

This structure simplifies maintenance and future expansion of the automation platform.

### Cross-System Automation

One of the primary goals of the project was to enable interaction between otherwise independent subsystems.

Examples include:

#### Security and Climate Control
* Cooling may be suspended when a protected window is opened.
* Heating behavior can be influenced by window status.

#### Security and Lighting
* Security events can activate lighting circuits.
* Presence simulation scenarios can be implemented.

#### Climate and Ventilation
* Ventilation operation can influence HVAC behavior.
* Future environmental-control scenarios can combine temperature and air-quality data.

Cross-system automation enables more intelligent behavior than isolated subsystem control.

### State Synchronization

The platform supports multiple control channels:
* Physical switches
* Web interface
* Telegram bot

All system states are synchronized automatically.

Regardless of where a command originates, all interfaces display the same current status.

This prevents inconsistent behavior and ensures predictable operation.

### Design Philosophy

Several principles guided the development of the automation engine:
* Simplicity
* Reliability
* Deterministic behavior
* Easy maintenance
* Expandability

The goal was not to create a fully generic automation framework, but rather a practical and dependable platform optimized for long-term residential use.

### Real-World Operation

The automation engine has been operating continuously in a real residential environment for several years.

During this period, numerous automation scenarios have been added, modified, and refined without requiring significant architectural changes.

This long-term operational experience has validated the flexibility and maintainability of the overall system architecture.
