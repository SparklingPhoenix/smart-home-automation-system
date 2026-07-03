// Execute secondary (long-press) actions assigned to physical switches.
void Secondary_Switch(int number)
{
    // Hallway switch.
    if (number == 1)
    {
        // If DoorBell() is currently active, cancel it instead of turning off all lights.
        if (Block_DBF_1 == 0 && DBF_1 != 0)
        {
            Block_DBF_1 = 1;                  // Prevent repeated execution.
            DoorBell_Function(0);             // Restore previous lighting state.
        }

        // Otherwise, switch off all lighting.
        else
        {
            BlackOut();
        }
    }

    // Bedroom / Wardrobe ventilation.
    if (number == 2 || number == 3)
        Bedroom_Wardrobe_Vent();

    // Bathroom ventilation.
    if (number == 4)
        Bathroom_Vent();

    // Living room service mode.
    if (number == 5)
        LivingRoom_Service_Mode();

    // Cabinet lighting shortcut.
    if (number == 6)
    {
        if (kit[7] != 0)
        {
            LocalSwitch_cabinet(0);
            Message_DB(7, 0, 1);
            kit[7] = 0;
        }
        else
        {
            LocalSwitch_cabinet(1);
            Message_DB(7, 1, 1);
            kit[7] = 1;
        }
    }

    // Kitchen lighting shortcut.
    if (number == 7)
    {
        if (kit[6] != 0)
        {
            LocalSwitch_kitchen(0);
            Message_DB(6, 0, 1);
            kit[6] = 0;
        }
        else
        {
            LocalSwitch_kitchen(1);
            Message_DB(6, 1, 1);
            kit[6] = 1;
        }
    }
}