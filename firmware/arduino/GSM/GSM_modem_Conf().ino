// GSM MODEM CONFIGURATION FUNCTION
void GSM_modem_Conf()
{
    //--------------------------------------------------------------------------
    // Check the modem status.
    //--------------------------------------------------------------------------

    // If the "GSM Call" option is enabled in the web interface,
    // start periodic modem status polling.
    if (Secur_index[7] == 1 || Secur_index[7] == -1)
    {
        if (Start_GSM_modem == 0)
        {
            // Start the polling timer.
            Start_GSM_modem = millis();

            // Request the modem operating status.
            Serial2.println("AT+CPAS");

            Serial.println("Request to GSM modem: AT+CPAS");
        }

        Current_GSM_modem = millis();

        // Wait for the modem response timeout.
        if (Current_GSM_modem - Start_GSM_modem >= GSM_modem_Check_1)
        {
            // Restart the polling timer.
            Start_GSM_modem = 0;

            if (Secur_index[7] == 1)
            {
                // Proceed to the modem startup sequence.
                Secur_index[7] = 2;
                Serial.println("Secur_index[7] = 2");
            }

            if (Secur_index[7] == -1)
            {
                // Proceed to the modem status verification sequence.
                Secur_index[7] = -2;
                Serial.println("Secur_index[7] = -2");
            }
        }
    }

    //--------------------------------------------------------------------------
    // Power up the GSM modem.
    //--------------------------------------------------------------------------

    if (Secur_index[7] == 2)
    {
        if (Start_GSM_modem == 0)
        {
            // Generate a 500 ms wake-up pulse.
            Start_GSM_modem = millis();

            digitalWrite(GSM_Wake, HIGH);
            delay(500);
            digitalWrite(GSM_Wake, LOW);

            Serial.println("0.5 sec wake-up pulse sent to GSM modem.");
        }

        Current_GSM_modem = millis();

        // Restart the startup procedure if the modem did not respond.
        if (Current_GSM_modem - Start_GSM_modem >= GSM_modem_Check_2)
        {
            Start_GSM_modem = 0;

            Secur_index[7] = 1;

            Serial.println("Modem startup failed. Restarting...");
        }
    }

    //--------------------------------------------------------------------------
    // Configure the GSM modem after successful network registration.
    //--------------------------------------------------------------------------

    if (Secur_index[7] == 3)
    {
        // Enable SMS text mode.
        Serial2.print("AT+CMGF=1\r");
        delay(300);

        // Enable software flow control.
        Serial2.print("AT+IFC=1,1\r");
        delay(300);

        // Select the SIM card phonebook.
        Serial2.print("AT+CPBS=\"SM\"\r");
        delay(300);

        // Configure incoming SMS notifications.
        Serial2.print("AT+CNMI=1,2,2,1,0\r");
        delay(300);

        // Enable Caller ID.
        Serial2.print("AT+CLIP=1\r");
        delay(300);

        // Select the GSM character set.
        Serial2.print("AT+CSCS=");
        delay(100);
        Serial2.println("\"GSM\"");
        delay(300);

        // Delete all SMS messages stored in the modem.
        Serial2.print("AT+CMGD=1,4\n");
        delay(500);

        Serial.println("GSM modem configured successfully.");

        // Configuration completed.
        Secur_index[7] = 4;
    }

    //--------------------------------------------------------------------------
    // Periodically monitor the modem status.
    //--------------------------------------------------------------------------

    if (Secur_index[7] == 4 || Secur_index[7] == -2)
    {
        if (Start_GSM_modem == 0)
        {
            // Start the periodic monitoring timer.
            Start_GSM_modem = millis();
        }

        Current_GSM_modem = millis();

        if (Current_GSM_modem - Start_GSM_modem >= GSM_modem_Check_3)
        {
            // Restart the monitoring timer.
            Start_GSM_modem = 0;

            // Alternate between different modem diagnostics.
            GSM_check++;

            if (Secur_index[7] == 4)
            {
                Secur_index[7] = 1;

                if (GSM_check == 1)
                {
                    // Request the GSM signal strength.
                    Serial2.println("AT+CSQ");
                }

                if (GSM_check == 2)
                {
                    // Request the network registration status.
                    Serial2.println("AT+CREG?");

                    GSM_check = 0;
                }

                delay(300);
            }

            if (Secur_index[7] == -2)
            {
                // Modem has been successfully powered down.
                Secur_index[7] = -1;

                Serial.println("Secur_index[7] = -1");

                // Reset GSM status values in the database.
                Message_DB(-4, 0, 21);
                Message_DB(-4, 0, 22);
                Message_DB(-4, 1, 23);
            }
        }
    }

    //--------------------------------------------------------------------------
    // Power down the GSM modem.
    //--------------------------------------------------------------------------

    // If the "GSM Call" option has been disabled,
    // generate a shutdown pulse.
    if (Secur_index[7] == 0)
    {
        digitalWrite(GSM_Wake, HIGH);
        delay(500);
        digitalWrite(GSM_Wake, LOW);

        delay(300);

        // Store the shutdown state.
        Secur_index[7] = -1;
    }
}