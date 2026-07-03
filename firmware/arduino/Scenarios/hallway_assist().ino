// AUTOMATIC HALLWAY LIGHTING ASSISTANCE
// Turns on the hallway lighting automatically when the entrance door is opened.
void hallway_assist()
{
    // Execute only when:
    // - the hallway lighting assistance feature is enabled,
    // - the security system is disarmed,
    // - no alarm condition is active.
    if (Secur_index[10] == 1 && Alarm_flag == 0)
    {
        // Run the scenario only once.
        if (SF_10 == 0)
        {
            // Turn on the hallway lighting.
            LocalSwitch_hallway(1);

            // Set the execution flag to prevent repeated activation.
            SF_10 = 1;

            // Store a special negative state.
            // During the next local switching cycle this forces the sequence
            // to start from position 0 ("All lights OFF"), effectively
            // preparing the lighting for automatic shutdown.
            kit[1] = -1;
        }
    }
}