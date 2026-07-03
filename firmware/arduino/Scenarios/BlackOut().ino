// Turn off all lighting groups included in the BlackOut scenario.
void BlackOut()
{
    // Iterate through all configured groups and zones.
    for (int i = 1; i < 11; i++)
    {
        // Skip groups excluded from the BlackOut scenario.
        if (Blackout_index[i] == 1)
        {
            // Reset the lighting preset for local lighting zones.
            if (i < 8)
                kit[i] = 0;

            // Apply the new state.
            Main_Switch(i);
        }
    }
}