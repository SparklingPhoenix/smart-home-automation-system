// Visual alarm indication ("alarm light show").
void Alarm_flicker()
{
    // Execute only when:
    // - mains power is available,
    // - the visual alarm indication feature is enabled.
    if (Power == 1 && Secur_index[13] == 1)
    {
        // Initialize the timer on the first execution.
        if (Start_Alarm_flicker == 0)
            Start_Alarm_flicker = millis();

        // Read the current system uptime.
        Current_Alarm_flicker = millis();

        // Is it time to switch the next output?
        if (Current_Alarm_flicker - Start_Alarm_flicker >= Offset && AF < 7)
        {
            // Advance to the next output in the sequence.
            AF++;

            // Increase the switching interval.
            Offset += 100;

            // Toggle the current lighting output.
            if (digitalRead(Flicker_list[AF]) == LOW)
                digitalWrite(Flicker_list[AF], HIGH);
            else
                digitalWrite(Flicker_list[AF], LOW);
        }

        // Reached the end of the sequence.
        if (AF == 7)
        {
            // Restart the animation cycle.
            AF = 0;
            Offset = 0;
            Start_Alarm_flicker = 0;
        }
    }
}