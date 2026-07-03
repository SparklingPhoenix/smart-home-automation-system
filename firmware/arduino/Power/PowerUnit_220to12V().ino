// Enable the 220V/12V power supply when at least one 12V load is switched ON.
void PowerUnit_220to12V(byte outPin, byte State)
{
    // List of outputs powered from the 12V supply.
    int Pins12V[9] =
    {
        0,
        out_11,
        out_21,
        out_31,
        out_41,
        out_51,
        out_61,
        howler,
        46
    };

    byte a = 0;

    // Turn the 12V power supply ON if:
    // - it is not locked due to overheating,
    // - a 12V load is being enabled,
    // - the power supply is currently OFF.
    if (PowerUnit_STOP == 0 &&
        State == 1 &&
        digitalRead(PowerUnit_12V) == LOW)
    {
        // Check whether the requested output belongs to the 12V group.
        for (byte p = 1; p < 9; p++)
        {
            if (Pins12V[p] == outPin)
                digitalWrite(PowerUnit_12V, HIGH);
        }

        // Allow the power supply output capacitor to charge.
        delay(30);
    }

    // Turn the 12V power supply OFF when a 12V load is disabled.
    if (State == 0 &&
        digitalRead(PowerUnit_12V) == HIGH)
    {
        // Count the remaining active 12V outputs.
        for (byte p = 1; p < 9; p++)
        {
            if (digitalRead(Pins12V[p]) == HIGH)
                a++;
        }

        // Disable the power supply if no active 12V loads remain.
        if (a == 0)
            digitalWrite(PowerUnit_12V, LOW);
    }
}