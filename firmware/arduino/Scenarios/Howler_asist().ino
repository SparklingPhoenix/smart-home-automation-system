// AUDIBLE CONFIRMATION OF SECURITY MODE CHANGES
// (12 V / 110 dB siren)
void Howler_asist()
{
    // Power up the 12 V supply if required.
    PowerUnit_220to12V(howler, 1);

    // Execute only if audible confirmation is enabled.
    if (Secur_index[5] == 1)
    {
        // Security mode armed.
        if (Security_State == 1)
        {
            // Wait one second before sounding the confirmation.
            delay(1000);

            // Generate two short confirmation beeps.
            for (int i = 1; i < 3; i++)
            {
                digitalWrite(howler, HIGH);
                delay(30);
                digitalWrite(howler, LOW);
                delay(170);
            }
        }

        // Security mode disarmed.
        if (Security_State == 0)
        {
            // Generate three short confirmation beeps.
            for (int i = 1; i < 4; i++)
            {
                digitalWrite(howler, HIGH);
                delay(30);
                digitalWrite(howler, LOW);
                delay(170);
            }
        }
    }

    // Power down the 12 V supply if it is no longer required.
    PowerUnit_220to12V(howler, 0);
}