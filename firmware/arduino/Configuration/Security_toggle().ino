// PROCESS SECURITY PANEL TOGGLE SWITCHES
void Security_toggle(int index)
{
    // Print the switch index to the serial monitor (debug).
    Serial.print("Secur_index[");
    Serial.print(index);

    // Toggle the current switch state.
    if (Secur_index[index] > 0)
    {
        // Switch OFF.
        Secur_index[index] = 0;

        Serial.println("]=0");

        // Store the new switch state in EEPROM.
        EEPROM.update(index, 0);
    }
    else
    {
        // Switch ON.
        Secur_index[index] = 1;

        Serial.println("]=1");

        // Store the new switch state in EEPROM.
        EEPROM.update(index, 1);
    }

    // Synchronize the new switch state with the database.
    Message_DB(-1, Secur_index[index], index);
}