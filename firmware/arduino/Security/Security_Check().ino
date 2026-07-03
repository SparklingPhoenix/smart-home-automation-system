// APARTMENT SECURITY SYSTEM MONITORING AND CONTROL
void Security_Check()
{
    //----------------------------------------------------------------------
    // Refresh the status of all security-related sensors.
    //----------------------------------------------------------------------

    // Update the debounce instances.
    debouncer[8].update();     // Valet push-button.
    debouncer[9].update();     // Doorbell push-button.
    debouncer[10].update();    // Lock bolt (deadbolt) position sensor.
    debouncer[11].update();    // Entrance door position sensor.
    debouncer[13].update();    // Living room window position sensor.

    //----------------------------------------------------------------------
    // LIVING ROOM WINDOW POSITION MONITORING
    //----------------------------------------------------------------------

    // Window has been opened.
    if (debouncer[13].read() == 1 && Window == 0)
    {
        // Store the new window state.
        Window = 1;

        // Automatically enable the bedroom/wardrobe ventilation if this
        // feature is enabled and the ventilation is currently OFF.
        if (Manag_index[10] == 1 && digitalRead(out_23) == LOW)
            Bedroom_Wardrobe_Vent();

        // Update the database.
        Message_DB(-1, 1, 11);
    }

    // Window has been closed.
    if (debouncer[13].read() == 0 && Window == 1)
    {
        // Store the new window state.
        Window = 0;

        // Automatically disable the bedroom/wardrobe ventilation if this
        // feature is enabled and the ventilation is currently ON.
        if (Manag_index[10] == 1 && digitalRead(out_23) == HIGH)
            Bedroom_Wardrobe_Vent();

        // Update the database.
        Message_DB(-1, 0, 11);
    }

    //----------------------------------------------------------------------
    // ENTRANCE DOOR LOCK (DEADBOLT) POSITION MONITORING
    //----------------------------------------------------------------------

    // The lock has been unlocked.
    if (debouncer[10].read() == 1 && GirthRail == 1)
    {
        // Store the unlocked state.
        GirthRail = 0;

        // Update the database.
        Message_DB(-1, 0, 3);
    }

    // The lock has been locked.
    if (debouncer[10].read() == 0 && GirthRail == 0)
    {
        // Store the locked state.
        GirthRail = 1;

        // Reset the "lock left open" timer.
        Start_label_GirthRail = 0;

        // Reset the doorbell reminder function.
        DoorBell_Function(0);

        // Update the database.
        Message_DB(-1, 1, 3);
    }

    //----------------------------------------------------------------------
    // ENTRANCE DOOR POSITION MONITORING
    //----------------------------------------------------------------------

    // The entrance door has been opened.
    if (debouncer[11].read() == 1 && InnerDoor == 1)
    {
        // Store the opened state.
        InnerDoor = 0;

        // Update the database.
        Message_DB(-1, 0, 4);
    }

    // The entrance door has been closed.
    if (debouncer[11].read() == 0 && InnerDoor == 0)
    {
        // Store the closed state.
        InnerDoor = 1;

        // Reset the "door left open" timer.
        Start_label_DoorState = 0;

        // Reset the doorbell reminder function.
        DoorBell_Function(0);

        // Update the database.
        Message_DB(-1, 1, 4);
    }

    //----------------------------------------------------------------------
    // DOORBELL PUSH-BUTTON MONITORING
    //----------------------------------------------------------------------

    // Someone has pressed the doorbell button.
    if (debouncer[9].read() == 0 && DoorButton == 0)
    {
        // Store the event flag.
        DoorButton = 1;

        // Send a Telegram notification.
        Telebot_message(5, 1);
    }
        //----------------------------------------------------------------------
    // TRACK POSSIBLE SEQUENCES OF DOOR / LOCK STATES
    // Used to determine the current security scenario.
    //----------------------------------------------------------------------

    if (GirthRail == 1 && InnerDoor == 0 && Step == 0)
        Step = 1;    // 1.1 Door open, lock engaged.

    if (GirthRail == 1 && InnerDoor == 1 && Step == 1)
        Step = 0;    // 2.1 Door closed, lock engaged.

    if (GirthRail == 0 && InnerDoor == 0 && Step == 1)
        Step = 2;    // 2.2 Door open, lock disengaged.

    if (GirthRail == 0 && InnerDoor == 0 && Step == 3)
        Step = 2;    // 2.3 Door open, lock disengaged.

    if (GirthRail == 0 && InnerDoor == 1 && Step == 2)
        Step = 3;    // 3.1 Door closed, lock disengaged.

    if (GirthRail == 1 && InnerDoor == 0 && Step == 2)
        Step = 1;    // 3.2 Door open, lock engaged.

    if (GirthRail == 1 && InnerDoor == 1 && Step == 1)
        Step = 0;    // 4.1 Everything closed.

    if (GirthRail == 1 && InnerDoor == 1 && Step == 3)
        Step = 4;    // 4.2 Everything closed.

    // 5. Door open, lock disengaged, security has just been disarmed.
    if (GirthRail == 0 && InnerDoor == 0 && Step == 6)
    {
        Step = 2;

        // Automatically turn on the hallway lighting if this feature is enabled.
        hallway_assist();
    }

    //----------------------------------------------------------------------
    // SECURITY SCENARIOS
    //----------------------------------------------------------------------

    // The lock has been forced while the security system is armed.
    // Trigger the alarm sequence.
    if (GirthRail == 0 && Security_State == 1)
    {
        // Enter ALARM state.
        Security_State = 2;

        // Allow the master phone call routine to execute.
        Call_flag = 0;
        GSM_Calling();

        // Activate the alarm indication.
        Alarm_flag = 1;

        // Reset the Valet mode flag.
        Valet_flag = 0;

        // Update the database.
        Message_DB(-1, 1, 2);

        // Store the current security state in EEPROM.
        EEPROM.put(0, Security_State);

        // Send a Telegram notification.
        Telebot_message(1, 2);

        // Enable the siren if audible alarm indication is enabled.
        if (Secur_index[14] == 1)
            digitalWrite(howler, HIGH);
    }

    //----------------------------------------------------------------------
    // Automatically arm the security system after the locking sequence
    // has been completed.
    //----------------------------------------------------------------------
    if (Step == 4)
        Security_State_func(1);

    //----------------------------------------------------------------------
    // ENTRANCE DOOR OPEN TIMER
    //----------------------------------------------------------------------

    // Start the reminder timer while the entrance door remains open.
    if (InnerDoor == 0 && Secur_index[6] == 1)
    {
        if (Start_label_DoorState == 0)
            Start_label_DoorState = millis();

        Current_label_DoorState = millis();

        // Notify the user if the configured timeout has elapsed.
        if (Current_label_DoorState - Start_label_DoorState >= SetPoint_DoorState)
            DoorBell_Function(1);
    }

    //----------------------------------------------------------------------
    // ENTRANCE DOOR LOCK OPEN TIMER
    //----------------------------------------------------------------------

    // Start the reminder timer while the lock remains unlocked.
    if (GirthRail == 0 && Secur_index[6] == 1)
    {
        if (Start_label_GirthRail == 0)
            Start_label_GirthRail = millis();

        Current_label_GirthRail = millis();

        // Notify the user if the configured timeout has elapsed.
        if (Current_label_GirthRail - Start_label_GirthRail >= SetPoint_GirthRail)
            DoorBell_Function(1);
    }

    //----------------------------------------------------------------------
    // Reset the DoorBell_Function() blocking flag.
    //----------------------------------------------------------------------

    // The blocking flag is cleared only after both the lock and the door
    // have returned to their normal (closed) state.
    if (Block_DBF_1 == 1)
    {
        if (GirthRail == 1 && InnerDoor == 1)
            Block_DBF_1 = 0;
    }

    //----------------------------------------------------------------------
    // DOORBELL BUTTON TIMER
    //----------------------------------------------------------------------

    if (DoorButton == 1)
    {
        if (Start_label_DoorButton == 0)
            Start_label_DoorButton = millis();

        Current_label_DoorButton = millis();

        // If the security system is disarmed, execute the doorbell indication.
        if (Security_State == 0)
            DoorBell_Function(1);

        // Restore the original lighting state after the configured timeout.
        if (Current_label_DoorButton - Start_label_DoorButton >= SetPoint_DoorButton)
        {
            DoorButton = 0;

            // Restore the previous lighting state.
            DoorBell_Function(0);

            // Reset the timer.
            Start_label_DoorButton = 0;
        }
    }

    //----------------------------------------------------------------------
    // Execute alarm indication effects.
    //----------------------------------------------------------------------
    if (Alarm_flag == 1)
        Alarm_flicker();

    //----------------------------------------------------------------------
    // Process the Valet push-button.
    //----------------------------------------------------------------------
    if (debouncer[8].read() == 0)
        Valet_Function();
}