// RECEIVING AND PROCESSING UART MESSAGES FROM THE NEOWAY M590 GSM MODEM
void GSM_modem_Receive_UART()
{
    // Process incoming data only while the GSM subsystem is enabled.
    if (Secur_index[7] > 0)
    {
        long ch_GSM_modem = 0;
        String val_GSM_modem = "";

        // Read the entire message received from the GSM modem.
        if (Serial2.available())
        {
            while (Serial2.available())
            {
                ch_GSM_modem = Serial2.read();
                val_GSM_modem += char(ch_GSM_modem);
                delay(30);
            }

            // Print the received message to the Serial Monitor.
            Serial.println(val_GSM_modem);
        }

        //----------------------------------------------------------------------
        // Process messages received from the GSM modem.
        //----------------------------------------------------------------------

        // The modem has completed its startup procedure.
        if (val_GSM_modem.indexOf("+PBREADY") > -1)
        {
            // Continue the modem initialization sequence.
            Secur_index[7] = 3;
        }

        // Retry the status request if the modem returned an error.
        if (val_GSM_modem.indexOf("ERROR") > -1)
        {
            delay(300);
            Serial2.println("AT+CPAS");
        }

        // Process the response to the "AT+CPAS" command.
        if (val_GSM_modem.indexOf("+CPAS:") > -1)
        {
            // Extract the modem status value.
            val_GSM_modem.remove(0, 17);
            val_GSM_modem.remove(1);

            uint8_t Value = val_GSM_modem.toInt();

            // Update the database.
            Message_DB(-4, Value, 23);

            val_GSM_modem = "";

            if (Value == 0)
            {
                // The modem is inactive.
                if (Secur_index[7] == -1)
                    Secur_index[7] = 0;

                // The modem is operational.
                if (Secur_index[7] == 1)
                    Secur_index[7] = 4;
            }
        }

        // Process the response to the "AT+CSQ" command.
        if (val_GSM_modem.indexOf("+CSQ: ") > -1)
        {
            // Extract the RSSI value.
            val_GSM_modem.remove(0, 15);

            int8_t L = val_GSM_modem.length() - 10;
            val_GSM_modem.remove(L);

            uint8_t Value = val_GSM_modem.toInt();

            // Convert RSSI (0...31) into a percentage.
            uint8_t value = map(Value, 0, 31, 0, 100);

            // Update the database.
            Message_DB(-4, value, 21);

            val_GSM_modem = "";
        }

        // Process the response to the "AT+CREG?" command.
        if (val_GSM_modem.indexOf("+CREG:") > -1)
        {
            // Extract the network registration status.
            val_GSM_modem.remove(0, 20);

            int8_t L = val_GSM_modem.length() - 7;
            val_GSM_modem.remove(L);

            uint8_t Val = val_GSM_modem.toInt();

            // Update the database.
            Message_DB(-4, Val, 22);

            val_GSM_modem = "";
        }

        // Process the USSD response containing the SIM balance.
        if (val_GSM_modem.indexOf("Balans ") > -1)
        {
            String Sub = val_GSM_modem.substring(19, 23);

            Serial.println(Sub);

            String Balans = "";
            Balans.reserve(60);

            // Update the SIM balance in the database.
            Balans += "mariadb s_h -e \"UPDATE p_m SET value=\'";
            Balans += Sub;
            Balans += "\' WHERE id=24;\"";

            Serial1.println(Balans);

            val_GSM_modem = "";
            Balans = "";
        }

        // Incoming voice call detected.
        if (val_GSM_modem.indexOf("RING") > -1)
        {
            // Enable Caller ID reporting.
            Serial2.print("AT+CLIP=1\r");
            delay(300);

            // Verify the caller's phone number.
            if (val_GSM_modem.indexOf(TEL_NUMBER) > -1)
            {
                val_GSM_modem = "";

                // Toggle the security system depending on its current state.
                switch (Security_State)
                {
                    case 0:
                        // Arm the security system.
                        Security_State_func(1);
                        break;

                    case 1:
                        // Disarm the security system.
                        Security_State_func(0);
                        break;

                    case 2:
                        // Disable both the alarm and the security mode.
                        Valet_Function();
                        break;
                }

                // Wait until the modem confirms that the call has been terminated.
                while (1)
                {
                    // Hang up the incoming call.
                    Serial2.println("ATH0");

                    if (Serial2.find("OK") ||
                        val_GSM_modem.indexOf("OK") > -1)
                        break;
                }
            }
            else
            {
                // Reject calls from unknown phone numbers.
                Serial2.println("ATH0");
            }

            // Clear the receive buffer.
            val_GSM_modem = "";
        }

        // Outgoing security notification call is currently in progress.
        if (Call_flag == 1)
        {
            // The destination answered the call.
            if (val_GSM_modem.indexOf("CONNECT") > -1)
            {
                // Hang up immediately after successful connection.
                Serial2.println("ATH0");

                val_GSM_modem = "";

                // Store the successful call status.
                Call_flag = 2;
            }
            else
            {
                // Connection failed. Allow another call attempt.
                Call_flag = 0;
            }
        }
    }
}