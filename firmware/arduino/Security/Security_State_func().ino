// ARM / DISARM SECURITY MODE.
void Security_State_func(byte index)
{
    // Update the current security mode.
    Security_State = index;

    // Print the new state to the serial monitor (debug).
    Serial.print("Security_State=");
    Serial.println(index);

    // Store the current security state in EEPROM
    // to restore it after a controller reboot.
    EEPROM.put(0, Security_State);

    // Update the security status in the database.
    Message_DB(-1, index, 1);

    // Generate an audible confirmation (if enabled).
    Howler_asist();

    // Notify the Telegram bot about the state change.
    Telebot_message(1, 1);

    // Turn off all configured loads when arming,
    // if this option is enabled in the settings.
    if (Secur_index[9] == 1)
        BlackOut();

    if (index == 1)
    {
        // Security mode activated.
        Step = 5;

        // Clear the Valet mode latch.
        Valet_flag = 0;

        // Reset the hallway lighting assistance event flag.
        if (SF_10 != 0)
            SF_10 = 0;
    }

    if (index == 0)
    {
        // Security mode deactivated.
        Step = 6;
    }
}