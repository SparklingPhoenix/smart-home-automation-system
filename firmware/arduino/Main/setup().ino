void setup()
{
    // ======================================================
    // Configure Digital Inputs
    // ======================================================

    for (int in = 1; in < 14; in++)
    {
        pinMode(input_pins[in], INPUT_PULLUP);     // Configure input with internal pull-up resistor
        debouncer[in].attach(input_pins[in]);      // Attach Bounce debouncer
        debouncer[in].interval(10);                // Debounce interval (ms)
    }


    // ======================================================
    // Configure Digital Outputs
    // ======================================================

    for (int out = 1; out < 40; out++)
    {
        pinMode(output_pins[out], OUTPUT);         // Configure output
        digitalWrite(output_pins[out], LOW);       // Initialize output to LOW
    }


    // ======================================================
    // Sensor Calibration
    // ======================================================

    voltageSensor.setSensitivity(SENSITIVITY);     // Calibrate AC voltage sensor

    sensor.calibrate();                            // Calibrate ACS712 current sensor


    // ======================================================
    // Initialize DHT22 Sensors
    // ======================================================

    digitalWrite(VCC_dht22, HIGH);                 // Power up DHT22 sensors

    dht1.begin();                                  // Indoor sensor
    dht2.begin();                                  // Outdoor sensor

    // ======================================================
    // Initialize UART Interfaces
    // ======================================================

    Serial.begin(57600);                           // Debug console
    delay(100);

    Serial1.begin(57600);                          // Orange Pi communication
    delay(100);

    Serial2.begin(57600);                          // GSM modem
    delay(100);

    Serial3.begin(9600);                           // Audio amplifier
    delay(500);

    // Serial.println("Hello");                    // Debug message

    // ======================================================
    // Restore Living Room Lighting Configuration
    // ======================================================

    Track_1 = EEPROM.read(101);                  // Lighting track #1
    Track_2 = EEPROM.read(102);                  // Lighting track #2
    Track_3 = EEPROM.read(103);                  // Lighting track #3
    Track_4 = EEPROM.read(104);                  // Lighting track #4


    // ======================================================
    // Restore Thermostat Setpoints
    // ======================================================

    EEPROM.get(105, Therm[1]);                   // Boiler thermostat setpoint
    EEPROM.get(107, Therm[2]);                   // Air conditioner thermostat setpoint
    EEPROM.get(109, Therm[6]);                   // Economy boiler thermostat setpoint
    EEPROM.get(111, Therm[7]);                   // Economy A/C thermostat setpoint


    // ======================================================
    // Restore Blackout Scenario Settings
    // ======================================================

    for (int ind = 1; ind < 11; ind++)
    {
        int address = ind + 120;

        uint8_t value = EEPROM.read(address);

        if (value < 2)                           // Ignore uninitialized EEPROM cells (0xFF)
        {
            Blackout_index[ind] = value;

            Serial.print("Blackout_index_");
            Serial.print(ind);
            Serial.print(": ");
            Serial.println(value);

            Message_DB(-3, value, ind);          // Synchronize with database

            delay(Delay_S1);                     // Prevent UART buffer overflow
        }
    }


    // ======================================================
    // Synchronize Universal Web Panel
    // ======================================================

    for (byte button = 1; button < 15; button++)
    {
        Message_DB(button, 0, 0);                // Update button state in the Web UI

        delay(Delay_S1);                         // Prevent UART buffer overflow
    }
}