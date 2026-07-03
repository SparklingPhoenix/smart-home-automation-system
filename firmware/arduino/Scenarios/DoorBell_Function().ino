// Audible and visual warning indication.
// Used for:
// - doorbell notification,
// - entrance door left open,
// - door lock left unlocked.
void DoorBell_Function(byte index)
{
    // Activate the warning function
    // (only if it is not blocked by the hallway switch).
    if (index == 1 && Block_DBF_1 == 0)
    {
        // Save the current lighting state so it can be restored later.
        if (digitalRead(out_13) == HIGH && DBF_1 == 0)
            DBF_1 = 1;
        if (digitalRead(out_13) == LOW && DBF_1 == 0)
            DBF_1 = 2;

        if (digitalRead(out_62) == HIGH && DBF_2 == 0)
            DBF_2 = 1;
        if (digitalRead(out_62) == LOW && DBF_2 == 0)
            DBF_2 = 2;

        // Start (or continue) the warning timer.
        if (Start_label_DoorBell == 0)
            Start_label_DoorBell = millis();

        Current_label_DoorBell = millis();

        // Energize the doorbell buzzer.
        digitalWrite(DoorBell, HIGH);

        // Has the configured interval elapsed?
        if (Current_label_DoorBell - Start_label_DoorBell >= SetPoint_DoorBell)
        {
            // Restart the timer.
            Start_label_DoorBell = 0;

            // Stop the buzzer.
            digitalWrite(DoorBell, LOW);

            // Flash the hallway night light.
            if (digitalRead(out_13) == HIGH)
                digitalWrite(out_13, LOW);
            else
                digitalWrite(out_13, HIGH);

            // Flash the bar counter lighting.
            if (digitalRead(out_62) == HIGH)
                digitalWrite(out_62, LOW);
            else
                digitalWrite(out_62, HIGH);
        }
    }

    // Restore the original state.
    if (index == 0)
    {
        // Turn off the doorbell buzzer.
        if (digitalRead(DoorBell) == HIGH)
            digitalWrite(DoorBell, LOW);

        // Restore the hallway night light.
        if (DBF_1 == 1)
            digitalWrite(out_13, HIGH);
        if (DBF_1 == 2)
            digitalWrite(out_13, LOW);

        // Restore the bar counter lighting.
        if (DBF_2 == 1)
            digitalWrite(out_62, HIGH);
        if (DBF_2 == 2)
            digitalWrite(out_62, LOW);

        // Clear the saved state flags.
        DBF_1 = 0;
        DBF_2 = 0;
    }
}