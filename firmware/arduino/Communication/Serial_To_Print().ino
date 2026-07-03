// Process commands received from the Arduino Serial Monitor
// and forward them to the appropriate UART interface.
void Serial_To_Print()
{
    long ch_serial = 0;
    String val_serial = "";

    // Has a command been entered in the Serial Monitor?
    if (Serial.available())
    {
        // Read the entire incoming string.
        while (Serial.available())
        {
            ch_serial = Serial.read();
            val_serial += char(ch_serial);
            delay(50);
        }

        // Print the received command for debugging.
        Serial.println(val_serial);

        // Forward a command to the OrangePi.
        if (val_serial.indexOf("O_Pi:") > -1)
        {
            // Remove the destination prefix.
            val_serial.remove(0, 5);

            // Send the command to the OrangePi.
            Serial1.println(val_serial);

            // Clear the command buffer.
            val_serial = "";
        }

        // Forward a command to the GSM modem.
        if (val_serial.indexOf("GSM:") > -1)
        {
            // Remove the destination prefix.
            val_serial.remove(0, 4);

            // Send the command to the GSM modem.
            Serial2.println(val_serial);

            // Print the transmitted command.
            Serial.print("to GSM-modem: ");
            Serial.println(val_serial);

            // Clear the command buffer.
            val_serial = "";
        }

        // Forward a command to the audio amplifier.
        if (val_serial.indexOf("Amp:") > -1)
        {
            // Remove the destination prefix.
            val_serial.remove(0, 4);

            // Send the command to the amplifier.
            // Amplifier.print(val_serial);

            // Clear the command buffer.
            val_serial = "";
        }
    }
}