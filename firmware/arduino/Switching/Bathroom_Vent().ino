// Enable or disable the bathroom ventilation system.
void Bathroom_Vent()
{
    // Flash the confirmation light if visual feedback is enabled.
    if (Manag_index[8] == 1)
    {
        // Skip indication if the window interlock prevents ventilation startup.
        if (!(Manag_index[11] == 1 && Window == 0 && digitalRead(out_44) == LOW))
        {
            // Flash the bathroom light.
            if (digitalRead(out_42) == LOW)
            {
                digitalWrite(out_42, HIGH);
                delay(100);
                digitalWrite(out_42, LOW);
            }
            else
            {
                digitalWrite(out_42, LOW);
                delay(100);
                digitalWrite(out_42, HIGH);
            }
        }
    }

    // Ventilation is currently OFF.
    if (digitalRead(out_44) == LOW)
    {
        // Do not start ventilation if the window interlock is active.
        if (!(Manag_index[11] == 1 && Window == 0))
        {
            digitalWrite(out_44, HIGH);      // Start ventilation.

            // Synchronize all user interfaces.
            Message_DB(0, 1, 13);
            Message_DB(8, 1, 0);
        }
    }

    // Ventilation is currently ON.
    else
    {
        digitalWrite(out_44, LOW);           // Stop ventilation.

        // Synchronize all user interfaces.
        Message_DB(0, 0, 13);
        Message_DB(8, 0, 0);
    }
}