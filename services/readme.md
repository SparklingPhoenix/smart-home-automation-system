# UART Listener

This lightweight Linux service continuously monitors the UART interface connected to the Arduino controller.

Whenever a command string is received, it executes the corresponding Linux command, allowing the low-level controller to invoke PHP scripts, Python applications, or other system utilities.

Communication parameters:
* UART: /dev/ttyS5
* Baud rate: 57600
* Format: 8N1

```text
     Arduino
        │
        ▼
 UART Listener (C)
        │
        ▼
      Linux
        ├── Apache
        ├── PHP
        ├── Python
        └── MariaDB
 ```
