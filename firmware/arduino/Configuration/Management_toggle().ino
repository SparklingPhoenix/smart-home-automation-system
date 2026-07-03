// PROCESS MANAGEMENT PANEL TOGGLE SWITCHES
void Management_toggle(int index)
{
    // Print the switch index to the serial monitor (debug).
    Serial.print("Manag_index[");
    Serial.print(index);

    // Calculate the EEPROM address assigned to this switch.
    int adress = index + 20;

    // Toggle the current switch state.
    if (Manag_index[index] > 0)
    {
        // Switch OFF.
        Manag_index[index] = 0;

        Serial.println("]=0");
    }
    else
    {
        // Switch ON.
        Manag_index[index] = 1;

        Serial.println("]=1");
    }

    // Store the current switch state.
    uint8_t Value = Manag_index[index];

    // Save the new state to EEPROM.
    EEPROM.update(adress, Value);

    // Synchronize the new switch state with the database.
    Message_DB(-2, Value, index);

    // Enforce mutually exclusive operating modes.
    // Cooling and heating modes cannot be enabled simultaneously.
    if (index == 6 && Manag_index[6] == 1 && Manag_index[12] == 1)
        Management_toggle(12);

    if (index == 6 && Manag_index[6] == 1 && Manag_index[4] == 1)
        Management_toggle(4);

    if (index == 12 && Manag_index[12] == 1 && Manag_index[6] == 1)
        Management_toggle(6);

    if (index == 4 && Manag_index[4] == 1 && Manag_index[6] == 1)
        Management_toggle(6);
}