# Telegram Bot

## Overview

The Telegram bot was not part of the original smart home design. At the time the project was started, I was not yet familiar with the capabilities of the Telegram Bot API. After the core automation system had already been completed and had been operating reliably for some time, I explored the Telegram API ecosystem and realized that a Telegram bot could become much more than an additional user interface—it could significantly improve the usability and accessibility of the entire system.

The Telegram interface provides several important advantages:

* Eliminates the need for a public IP address by using Telegram's cloud infrastructure as a communication channel.
* Benefits from Telegram's mature security architecture, reducing the need to expose any services directly to the Internet.
* The Telegram Bot API is free for personal use.
* A large developer community continuously maintains and improves the Telegram ecosystem.
* Bots can be added to Telegram groups and channels, making future multi-user interaction possible.

The Telegram bot provides full remote access to the smart home system, allowing the apartment to be monitored and controlled from anywhere with an Internet connection.

Typical use cases include:

* Preheating the apartment during winter before arriving home.
* Starting the air conditioner during summer in advance.
* Disarming the security system while approaching the building.
* Receiving notifications about mains power failures.
* Receiving doorbell notifications.
* Receiving security alarm notifications.
* Starting the ventilation system remotely.
* Turning lights on remotely to simulate occupancy.

The bot is implemented in **Python** using the **pyTelegramBotAPI** library.

In addition to interactive bot messages, the system generates asynchronous push notifications using PHP scripts. These scripts create HTTP requests to the Telegram Bot API and are triggered directly by low-level automation events executed on the Arduino controller.

Unlike interactive menu messages, push notifications remain in the conversation history together with their original timestamps, providing a convenient event log.

For example, pressing the apartment doorbell immediately generates a Telegram message containing the text:
> **Knock knock!**
<p align="center">
  <img src="/images/screenshot/Telegram-bot-push-notification-doorbell.jpg" height="400">
</p>

![▶ Web-interface switch panel simplified](https://github.com/user-attachments/assets/d4832d70-9399-4015-901b-4f354dae187b)

---

# Main Menu

The bot can be initialized either by sending the `/start` command or by clearing the previous conversation history.

After initialization, the bot sends a welcome message and generates a new set of virtual keyboard buttons.
<p align="center">
  <img src="/images/screenshot/Telegram-bot-menu-main.jpg" height="400">
</p>

The main menu contains four primary sections:
* Climate Control
* Energy Monitoring
* Security
* Lighting

Whenever the user navigates to another submenu, the bot deletes the previous message and sends a new one.

This behavior is intentional. The Telegram Bot API requires a new message to generate a new virtual keyboard layout.

The labels of the virtual buttons are generated dynamically according to the current system state.

For example, if the security system is currently armed, the corresponding button will display **"Disarm Security System"** instead of **"Arm Security System"**, making the interface context-aware.

---

# Climate Control Menu

<p align="center">
  <img src="/images/screenshot/Telegram-bot-menu-climate-control.jpg" height="400">
</p>

The first-level **Climate Control** submenu provides real-time environmental information, including:
* Indoor temperature and humidity
* Outdoor temperature and humidity
* Current gas boiler status
* Current air conditioner status
* Window position (open or closed)

The submenu provides the following navigation buttons:
* Gas Boiler
* Air Conditioner
* Main Menu
* Refresh

Selecting **Refresh** deletes the current message and requests fresh data from the controller before displaying an updated status page.

---

# Gas Boiler Menu

<p align="center">
  <img src="/images/screenshot/Telegram-bot-menu-gasboiler-thermostat.jpg" height="400">
</p>

The **Gas Boiler** submenu provides complete information about the heating system and thermostat operation.

Displayed information includes:
* Thermostat status (Enabled / Disabled)
* Current comfort temperature setpoint
* Economy mode status
* Current economy temperature setpoint
* Measured indoor temperature swing during the most recent boiler cycle
* Duration of the previous heating cycle (typically 30–80 minutes during winter)
* Additional reference information explaining the priority between Comfort Mode and Economy Mode

Available actions include:
* Enable or disable the thermostat
* Change the comfort temperature setpoint
* Enable or disable Economy Mode
* Change the Economy Mode temperature setpoint
* Return to the Climate Control menu
* Refresh current data

Selecting either **Comfort Setpoint** or **Economy Setpoint** starts an interactive conversation with the bot.

The bot requests a new temperature value, validates the entered data, and reports formatting errors if necessary before requesting another input.

<p align="center">
  <img src="/images/screenshot/Telegram-bot-menu-gasboiler-thermostat-value.jpg" height="400">
  <img src="/images/screenshot/Telegram-bot-menu-gasboiler-thermostat-wrong-value.jpg" height="400">
</p>

![▶ Web-interface switch panel simplified](https://github.com/user-attachments/assets/490d92ca-2ac8-4ce7-a3c8-bc314d4553c4)

Whenever the boiler is started or stopped, the Arduino controller executes the corresponding automation scenario and the system generates an independent push notification through the PHP notification service, confirming successful execution of the requested operation.

---

## Air Conditioner Menu

<p align="center">
  <img src="/images/screenshot/Telegram-bot-menu-AC-thermostat.jpg" height="400">
</p>

![▶ Web-interface switch panel simplified](https://github.com/user-attachments/assets/518d1a1c-bbf4-4d6d-9296-67c6b0fde1e3)

The **Air Conditioner** submenu follows the same structure and operating principles as the **Gas Boiler** menu. All displayed information and available commands relate exclusively to the air conditioning system.

The submenu displays the following information:
* Air conditioner thermostat status (Enabled / Disabled)
* Current temperature setpoint
* Economy Mode status
* Economy Mode temperature setpoint
* Reference information explaining the priority between Comfort Mode and Economy Mode

The available actions are identical to those provided for the gas boiler, including enabling or disabling the thermostat and modifying both comfort and economy temperature setpoints.

---

## Energy Monitoring Menu

The **Energy Monitoring** submenu provides real-time information about the electrical supply and power consumption of the automation system.

<p align="center">
  <img src="/images/screenshot/Telegram-bot-menu-power-auditing.jpg" height="400">
</p>

The following parameters are displayed for the AC mains supply:
* Supply voltage
* Load current
* Active power consumption

The following parameters are displayed for the 12 V power supply:
* Output current
* Output power

The submenu also reports the current status of several automation modes:
* Energy Saving Mode
* Automatic activation of Energy Saving Mode when powered from a backup power station
* Ventilation lock while Energy Saving Mode is active

This submenu is intended for monitoring only.

Available buttons include:
* Main Menu
* Refresh

No configuration changes can be made from this menu.

Whenever the mains power supply is lost or restored, the system automatically generates a push notification.

Since Telegram preserves message timestamps, the duration of a power outage can be determined directly from the conversation history.

---

## Security Menu

The **Security** submenu provides complete information about the current status of the apartment security system.

<p align="center">
  <img src="/images/screenshot/Telegram-bot-menu-security.jpg" height="400">
</p>

![▶ Web-interface switch panel simplified](https://github.com/user-attachments/assets/07503845-2d63-445e-8bb7-bff9afcd10c6)

Displayed information includes:
* Security mode status
* Alarm status
* Entrance door lock status
* Entrance door status
* Ventilation window status

The menu also displays the current state of several automation features associated with the security system:
* Automatic arming when the entrance door lock reaches the fully locked position.
* Audible confirmation using the indoor siren when the security mode changes.
* Reminder if the entrance door or lock has not been properly secured.
* GSM-based arming/disarming and alarm call notifications.
* Automatic hallway lighting ("Light Assistant") after disarming the apartment.
* Automatic shutdown of selected lighting circuits when the apartment is armed.

Available actions include:
* Arm / Disarm Security System
* Activate / Clear Alarm
* Main Menu
* Refresh

Changing the security state starts the corresponding low-level automation scenario on the Arduino controller.

After successful execution, a PHP notification service running on the Orange Pi generates a separate Telegram notification confirming completion of the requested operation.

When the security system is armed, the notification additionally contains:
* Current outdoor temperature
* Blackout scenario execution status
* Window status

When the security system is disarmed, the notification contains:
* Current indoor temperature
* Updated security status

This notification serves as independent confirmation that the requested operation has been successfully executed by the automation controller.

---

## Lighting Menu

The **Lighting** submenu displays the current status of every lighting circuit and ventilation output installed throughout the apartment.

(The message also displays the current status of an integrated Bluetooth audio system. Since this subsystem belongs to a separate project, it is not documented within this repository.)

<p align="center">
  <img src="/images/screenshot/Telegram-bot-menu-light-switch.jpg" height="400">
</p>

The bot dynamically generates virtual buttons corresponding to every controllable lighting and ventilation circuit.

At the bottom of the menu, two standard navigation buttons are always available:

* Main Menu
* Refresh

When a lighting circuit is active, the corresponding virtual button displays a light bulb icon together with its label, providing immediate visual feedback.

![▶ Web-interface switch panel simplified](https://github.com/user-attachments/assets/0abbe68e-18c0-45dd-bbd3-0e1a491e325e)
![▶ Web-interface switch panel simplified](https://github.com/user-attachments/assets/b805f32d-54c1-4595-8d5e-b58f31afc3ad)

For simplicity and everyday usability, the Telegram interface controls only the **local lighting** circuits rather than exposing every individual lighting output available through the web interface.


## Conclusion

The Telegram bot complements the local web interface by providing secure and convenient remote access to the most frequently used smart home functions.

Its primary purpose is operational control and real-time notifications, while full system configuration and advanced settings remain available through the local web interface.
