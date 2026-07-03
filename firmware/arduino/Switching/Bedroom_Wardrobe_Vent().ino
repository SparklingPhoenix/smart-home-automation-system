// Enable or disable the bedroom / wardrobe ventilation system.
void Bedroom_Wardrobe_Vent()
{
    // Flash indicator lights if visual confirmation is enabled.
    if (Manag_index[8] == 1)
    {
        // Skip indication if ventilation is blocked by the window interlock.
        if (!(Manag_index[9] == 1 && Window == 0 && digitalRead(out_23) == LOW))
        {
            // Flash the wardrobe light.
            if (digitalRead(out_32) == LOW)
            {
                digitalWrite(out_32, HIGH);
                delay(50);
                digitalWrite(out_32, LOW);
            }
            else
            {
                digitalWrite(out_32, LOW);
                delay(100);
                digitalWrite(out_32, HIGH);
            }

            // Flash the hallway light.
            if (digitalRead(out_13) == LOW)
            {
                digitalWrite(out_13, HIGH);
                delay(50);
                digitalWrite(out_13, LOW);
            }
            else
            {
                digitalWrite(out_13, LOW);
                delay(100);
                digitalWrite(out_13, HIGH);
            }
        }
    }

    // Ventilation is currently OFF.
    if (digitalRead(out_23) == LOW)
    {
        // Do not start ventilation if the window interlock is active.
        if (!(Manag_index[9] == 1 && Window == 0))
        {
            digitalWrite(out_23, HIGH);      // Start ventilation.

            Start_BedRoom_Vent = 0;          // Reset the ventilation timer.

            // Synchronize all user interfaces.
            Message_DB(0, 1, 6);
            Message_DB(0, 1, 9);
            Message_DB(9, 1, 0);
        }
    }

    // Ventilation is currently ON.
    else
    {
        digitalWrite(out_23, LOW);           // Stop ventilation.

        // Synchronize all user interfaces.
        Message_DB(0, 0, 6);
        Message_DB(0, 0, 9);
        Message_DB(9, 0, 0);
    }
}