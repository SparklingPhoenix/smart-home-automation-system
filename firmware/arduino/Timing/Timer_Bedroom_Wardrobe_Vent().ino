// Timer for automatic shutdown of the bedroom / wardrobe ventilation.
void Timer_Bedroom_Wardrobe_Vent(uint32_t SetPoint_BedRoom_Vent)
{
    // Execute only if the automatic ventilation timer is enabled
    // and the ventilation is currently running.
    if (Manag_index[15] == 1 && digitalRead(out_23) == HIGH)
    {
        // Store the timer start timestamp.
        if (Start_BedRoom_Vent == 0)
            Start_BedRoom_Vent = millis();

        // Read the current system uptime.
        Current_BedRoom_Vent = millis();

        // Has the configured ventilation time elapsed?
        if (Current_BedRoom_Vent - Start_BedRoom_Vent >= SetPoint_BedRoom_Vent)
        {
            // Toggle the ventilation state (turn it OFF).
            Bedroom_Wardrobe_Vent();
        }
    }
}