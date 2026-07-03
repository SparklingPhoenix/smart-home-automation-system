// BUILDS AND SENDS USSD REQUESTS TO THE MOBILE OPERATOR VIA THE GSM MODEM
void USSD_request(uint8_t arg)
{
    // String buffer for the outgoing USSD request.
    String USSD = "";

    // Execute only if the GSM modem is enabled.
    if (Secur_index[7] > 0)
    {
        // Build the requested USSD command.
        if (arg == 1)
        {
            // Request the account balance.
            USSD = "ATD#111#";
        }

        // Send the USSD request to the GSM modem.
        Serial2.println(USSD);

        // Print the request to the Serial Monitor for debugging.
        Serial.println(USSD);
    }
}