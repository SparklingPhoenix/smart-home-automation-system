// SERIAL COMMUNICATION WITH OrangePi
// RECEIVE AND PROCESS UART MESSAGES FROM OrangePi
void OrangePi_Receive_UART()
{
    long ch_OrangePi = 0;
    String val_OrangePi = "";

    // Read an incoming message from OrangePi.
    if (Serial1.available())
    {
        while (Serial1.available())
        {
            ch_OrangePi = Serial1.read();
            val_OrangePi += char(ch_OrangePi);
            delay(30);
        }

        // Print the received message to the Serial Monitor.
        Serial.println(val_OrangePi);
    }

    //-------------------------------------------------------------------------
    // Process commands received from OrangePi.
    //-------------------------------------------------------------------------

    // Manual lighting control panel button.
    if (val_OrangePi.indexOf("btn_") > -1)
    {
        val_OrangePi.remove(0, 6);

        byte Array_cell = val_OrangePi.toInt();

        // Execute the corresponding manual control command.
        Manual_Switch_Panel(Array_cell);
    }

    // Universal control panel button.
    if (val_OrangePi.indexOf("button_") > -1)
    {
        val_OrangePi.remove(0, 7);

        int Zone = val_OrangePi.toInt();

        // Execute the corresponding universal control command.
        Universal_Switch_Panel(Zone);
    }

    // Telegram bot button.
    if (val_OrangePi.indexOf("tel_bt_") > -1)
    {
        val_OrangePi.remove(0, 7);

        byte Zone = val_OrangePi.toInt();

        // Execute the corresponding universal control command.
        Universal_Switch_Panel(Zone);

        // Set a flag to send the amplifier status back to Telegram.
        if (Zone == 10 || Zone == 11)
            Amp_TB = 1;
    }

    // Security system activation/deactivation request.
    if (val_OrangePi.indexOf("Security_State") > -1)
    {
        switch (Security_State)
        {
            case 0:
                // The security system is currently disarmed.
                // Arm it only if the entrance door is locked.
                if (GirthRail == 1)
                    Security_State_func(1);
                else
                    Telebot_message(1, 3);
                break;

            case 1:
                // Disarm the security system.
                Security_State_func(0);
                break;

            case 2:
                // Alarm condition is active.
                // Disable both the alarm and the security system.
                Valet_Function();
                break;
        }
    }

    // Alarm activation/deactivation request.
    if (val_OrangePi.indexOf("Alarm_State") > -1)
    {
        if (Alarm_flag == 0)
        {
            // Update the database.
            Message_DB(-1, 1, 2);

            // Allow the previous database command to complete.
            delay(200);

            Alarm_flag = 1;

            // Clear the Valet mode flag.
            Valet_flag = 0;

            // Notify the Telegram bot.
            Telebot_message(1, 2);
        }
        else
        {
            // Disable the alarm.
            Valet_Function();
        }
    }
        // Security settings panel switch.
    if (val_OrangePi.indexOf("Security_toggle_") > -1)
    {
        val_OrangePi.remove(0, 16);

        int index = val_OrangePi.toInt();

        if (index == 1)
        {
            // Toggle the security system.
            if (Security_State == 0)
            {
                // Arm the system only if the entrance door is locked.
                if (GirthRail == 1)
                    Security_State_func(1);
                else
                    Telebot_message(1, 3);
            }
            else
            {
                // Disarm the security system.
                Security_State_func(0);
            }
        }

        if (index == 2)
        {
            // Toggle the alarm mode.
            if (Alarm_flag == 0)
            {
                // Update the database.
                Message_DB(-1, 1, 2);

                // Allow the previous database command to complete.
                delay(200);

                Alarm_flag = 1;

                // Clear the Valet mode flag.
                Valet_flag = 0;

                Security_toggle(index);

                // Notify the Telegram bot.
                Telebot_message(1, 2);
            }
            else
            {
                // Disable the alarm.
                Valet_Function();
            }
        }

        // Other security panel switches.
        if (index > 2)
            Security_toggle(index);
    }

    // Blackout settings panel switch.
    if (val_OrangePi.indexOf("Blackout_toggle_") > -1)
    {
        val_OrangePi.remove(0, 16);

        int index = val_OrangePi.toInt();

        Blackout_toggle(index);
    }

    // Energy management settings panel switch.
    if (val_OrangePi.indexOf("Manag_toggle_") > -1)
    {
        val_OrangePi.remove(0, 13);

        int index = val_OrangePi.toInt();

        Management_toggle(index);
    }

    // Telegram bot management switch.
    if (val_OrangePi.indexOf("tel_Mt_") > -1)
    {
        val_OrangePi.remove(0, 7);

        byte index = val_OrangePi.toInt();

        Management_toggle(index);

        // Notify the Telegram bot.
        Telebot_message(2, index);
    }

    // Thermostat slider from the Energy Management panel.
    if (val_OrangePi.indexOf("Therm_") > -1)
    {
        String Therm = val_OrangePi;

        Therm.remove(0, 6);
        Therm.remove(1, 5);

        // Determine the thermostat index.
        byte index = Therm.toInt();

        val_OrangePi.remove(0, 8);

        // Read the requested temperature value.
        float value = val_OrangePi.toFloat();

        Thermostat_values(index, value);

        // Clear the temporary string.
        Therm = "";
    }

    // Thermostat value received from the Telegram bot.
    if (val_OrangePi.indexOf("tel_therm_") > -1)
    {
        String Therm = val_OrangePi;

        Therm.remove(0, 10);
        Therm.remove(1, 5);

        // Determine the thermostat index.
        uint8_t index = Therm.toInt();

        val_OrangePi.remove(0, 12);

        // Read the requested temperature value.
        float value = val_OrangePi.toFloat();

        Thermostat_values(index, value);

        // Notify the Telegram bot.
        Telebot_message(3, index);

        // Clear the temporary string.
        Therm = "";
    }

    // Time-stepper control received from the Energy Management panel.
    if (val_OrangePi.indexOf("excerpt_") > -1)
    {
        String Excerpt = val_OrangePi;

        Excerpt.remove(0, 8);
        Excerpt.remove(2);

        // Determine the stepper index.
        int index = Excerpt.toInt();

        val_OrangePi.remove(0, 11);

        // Read the requested timer value.
        uint32_t value = val_OrangePi.toInt();

        Time_stepper(index, value, 1);

        // Clear the temporary string.
        Excerpt = "";
    }

    // GSM panel button received from the web interface.
    if (val_OrangePi.indexOf("gsm_Btn_") > -1)
    {
        val_OrangePi.remove(0, 8);

        uint8_t arg = val_OrangePi.toInt();

        // Execute the corresponding GSM command.
        USSD_request(arg);
    }

    // Clear the receive buffer.
    val_OrangePi = "";
}