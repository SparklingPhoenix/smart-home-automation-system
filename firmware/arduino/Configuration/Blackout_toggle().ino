// PROCESS BLACKOUT PANEL TOGGLE SWITCHES
void Blackout_toggle(int index)
{
    // Print the switch index to the serial monitor (debug).
    Serial.print("Blackout_index[");
    Serial.print(index);

    // Calculate the EEPROM address assigned to this switch.
    int adress = index + 120;

    // Toggle the current switch state.
    if (Blackout_index[index] > 0)
    {
        // Switch OFF.
        Blackout_index[index] = 0;

        Serial.println("]=0");
    }
    else
    {
        // Switch ON.
        Blackout_index[index] = 1;

        Serial.println("]=1");
    }

    // Store the current switch state.
    uint8_t Value = Blackout_index[index];

    // Save the new state to EEPROM.
    EEPROM.update(adress, Value);

    // Synchronize the new switch state with the database.
    Message_DB(-3, Value, index);
}