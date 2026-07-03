// Deactivate the alarm state (Valet mode).
void Valet_Function()
{
    // Execute the procedure only once.
    if (Valet_flag == 0)
    {
        // Latch the function to prevent repeated execution.
        Valet_flag = 1;

        // Clear the alarm indication flag.
        Alarm_flag = 0;

        // Turn off the siren.
        digitalWrite(howler, LOW);

        // Turn off all lighting if the visual alarm indication
        // ("alarm light show") was active.
        if (Secur_index[13] == 1)
            BlackOut();

        // Update the database: alarm deactivated.
        Message_DB(-1, 0, 2);

        // Reset the security mode.
        Security_State = 0;

        // Save the new security state to EEPROM.
        EEPROM.put(0, Security_State);

        // Update the database: security mode disabled.
        Message_DB(-1, 0, 1);

        // Debug message.
        Serial.println("Valet");

        // Notify the Telegram bot:
        // - alarm deactivated,
        // - security mode disabled.
        Telebot_message(1, 2);
        Telebot_message(1, 1);
    }
}