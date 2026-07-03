// Timer for automatic shutdown of the bathroom ventilation.
void Timer_BathRoom_Vent(uint32_t SetPoint_Vent)
{
    // Execute only when:
    // - the bathroom lighting zone is OFF,
    // - the ventilation is running,
    // - the automatic timer is enabled.
    if (kit[4] == 0 &&
        digitalRead(out_44) == HIGH &&
        Manag_index[16] == 1)
    {
        // Store the timer start timestamp.
        if (Start_BathRoom_Vent == 0)
            Start_BathRoom_Vent = millis();

        // Read the current system uptime.
        Current_BathRoom_Vent = millis();

        // Has the configured runtime elapsed?
        if (Current_BathRoom_Vent - Start_BathRoom_Vent >= SetPoint_BathRoom_Vent)
        {
            // Reset the timer.
            Start_BathRoom_Vent = 0;

            // Stop ventilation.
            Bathroom_Vent();
        }
    }
}