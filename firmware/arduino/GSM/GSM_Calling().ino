// GSM CALL TO THE MASTER PHONE NUMBER
void GSM_Calling()
{
    // Execute only if the GSM subsystem is enabled.
    if (Secur_index[7] > 0)
    {
        // Prevent repeated call attempts while a call is already in progress.
        if (Call_flag == 0)
        {
            // Store the outgoing call state.
            Call_flag = 1;

            Serial.println("GSM_Calling()");

            // Dial the predefined master phone number.
            Serial2.print("ATD+");
            Serial2.print(TEL_NUMBER);
            Serial2.println(";");

            // Allow the modem time to process the command.
            delay(Delay_S1);
        }
    }
}