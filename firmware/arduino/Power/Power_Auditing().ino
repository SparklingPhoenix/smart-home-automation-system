// MONITOR AC MAINS ELECTRICAL PARAMETERS
void Power_Auditing()
{
    // Collect measurement samples until the calculation interval expires.
    if (Compute == 0)
    {
        // Increment the sample counter.
        N++;

        // Measure AC mains voltage.
        float adc_sample = voltageSensor.getRmsVoltage();

        // Accumulate the squared voltage samples for RMS calculation.
        sum += sq(adc_sample);

        // Measure AC load current.
        float Value_AC = sensor.getCurrentAC();

        // Accumulate the squared current samples for RMS calculation.
        i += sq(Value_AC);

        // Measure DC current.
        // The average value will be calculated later.
        Value_DC += analogRead(Current_Sens);
    }

    // Start the measurement timer.
    if (Start_Power_Measure == 0)
        Start_Power_Measure = millis();

    // Calculate the AC mains voltage when the sampling interval expires.
    if ((millis() - Start_Power_Measure) >= SetPoint_Power_check &&
        Compute == 0)
    {
        Compute = 1;

        // Calculate the RMS voltage.
        volt = sqrt(sum / N);

        // Suppress noise when no mains voltage is present.
        if (volt < 20)
            volt = 0;

        // Check whether the mains voltage is within the acceptable range.
        if (volt > 198 && volt < 245)
            Power = 1;
        else
            Power = 0;

        // Format the value with one decimal place.
        String Volt(volt, 1);

        // Build the database update command.
        String Message_to_p_m;
        Message_to_p_m.reserve(60);

        Message_to_p_m += "mariadb s_h -e \"UPDATE p_m SET value=\'";
        Message_to_p_m += Volt;
        Message_to_p_m += "\' WHERE id=1;\"";

        // Send the measured voltage to the database.
        Serial1.println(Message_to_p_m);

        Message_to_p_m = "";

        // Reset the accumulated voltage samples.
        sum = 0;
    }

    // Calculate the AC load current one second later.
    if ((millis() - Start_Power_Measure) >= SetPoint_Power_check + 1000 &&
        Compute == 1)
    {
        Compute = 2;

        // Calculate the RMS current.
        I_ac = sqrt(i / N);

        // Build the database update command.
        String Message_to_p_m;
        Message_to_p_m.reserve(60);

        Message_to_p_m += "mariadb s_h -e \"UPDATE p_m SET value=\'";
        Message_to_p_m += I_ac;
        Message_to_p_m += "\' WHERE id=2;\"";

        // Send the measured current to the database.
        Serial1.println(Message_to_p_m);

        Message_to_p_m = "";

        // Reset the accumulated current samples.
        i = 0;
    }
    
        // Calculate the DC load current one second later.
    if ((millis() - Start_Power_Measure) >= SetPoint_Power_check + 2000 &&
        Compute == 2)
    {
        Compute = 3;

        // Calculate the average ADC reading.
        uint32_t sensorValue = (Value_DC / N) - 12;

        // Convert the ADC reading to the sensor output voltage.
        float voltage = sensorValue * (5.217) / 1024.0;

        // Convert the sensor output voltage to the primary-side DC current.
        I_dc = (voltage - 2.61) / 0.100;

        // Suppress the sensor idle offset.
        if (I_dc * 1000 < 80)
            I_dc = 0;

        // Build the database update command.
        String Message_to_p_m;
        Message_to_p_m.reserve(60);

        Message_to_p_m += "mariadb s_h -e \"UPDATE p_m SET value=\'";
        Message_to_p_m += I_dc;
        Message_to_p_m += "\' WHERE id=3;\"";

        // Send the measured DC current to the database.
        Serial1.println(Message_to_p_m);

        Message_to_p_m = "";

        // Reset the accumulated DC current samples.
        Value_DC = 0;
    }

    // Calculate the AC power one second later.
    if ((millis() - Start_Power_Measure) >= SetPoint_Power_check + 3000 &&
        Compute == 3)
    {
        Compute = 4;

        // Calculate the apparent AC power at the distribution panel input.
        float P_ac = volt * I_ac;

        // Format the value with one decimal place.
        String Pwr_ac(P_ac, 1);

        // Build the database update command.
        String Message_to_p_m;
        Message_to_p_m.reserve(60);

        Message_to_p_m += "mariadb s_h -e \"UPDATE p_m SET value=\'";
        Message_to_p_m += Pwr_ac;
        Message_to_p_m += "\' WHERE id=4;\"";

        // Send the calculated AC power to the database.
        Serial1.println(Message_to_p_m);

        Message_to_p_m = "";
    }

    // Calculate the DC power and finalize the measurement cycle.
    if ((millis() - Start_Power_Measure) >= SetPoint_Power_check + 4000 &&
        Compute == 4)
    {
        // Start a new measurement cycle.
        Compute = 0;

        // Calculate the DC power supplied by the 12 V power supply.
        float P_dc = 12 * I_dc;

        // Check for power supply overload.
        if (P_dc > P_acdc_max)
            PowerUnit_STOP = 1;
        else
            PowerUnit_STOP = 0;

        // Format the value with one decimal place.
        String Pwr_dc(P_dc, 1);

        // Build the database update command.
        String Message_to_p_m;
        Message_to_p_m.reserve(60);

        Message_to_p_m += "mariadb s_h -e \"UPDATE p_m SET value=\'";
        Message_to_p_m += Pwr_dc;
        Message_to_p_m += "\' WHERE id=5;\"";

        // Send the calculated DC power to the database.
        Serial1.println(Message_to_p_m);

        Message_to_p_m = "";

        // Reset the sample counter.
        N = 0;

        // Restart the measurement timer.
        Start_Power_Measure = 0;
    }

        // Power-related automation scenarios.
    if (Manag_index[3] > 0)
    {
        // Ventilation power-failure scenario.

        // AC mains power lost.
        if (Power == 0)
        {
            // Stop the bedroom/wardrobe ventilation if it was running.
            if (digitalRead(out_23) == HIGH)
            {
                Bedroom_Wardrobe_Vent();
                Vent_Bed = 1;      // Remember that it was stopped automatically.
            }

            // Stop the bathroom ventilation if it was running.
            if (digitalRead(out_44) == HIGH)
            {
                Bathroom_Vent();
                Vent_Bath = 1;     // Remember that it was stopped automatically.
            }
        }

        // AC mains power restored.
        if (Power == 1)
        {
            // Restore the bedroom/wardrobe ventilation.
            if (digitalRead(out_23) == LOW && Vent_Bed == 1)
            {
                Bedroom_Wardrobe_Vent();
                Vent_Bed = 0;
            }

            // Restore the bathroom ventilation.
            if (digitalRead(out_44) == LOW && Vent_Bath == 1)
            {
                Bathroom_Vent();
                Vent_Bath = 0;
            }
        }
    }

    // Automatic ECO mode activation after a power outage.
    if (Manag_index[2] == 1)
    {
        // Enable ECO mode while running on backup power.
        if (Power == 0 && Manag_index[1] == 0)
        {
            Manag_index[1] = 1;

            // Synchronize the new state with the database.
            Message_DB(-2, 1, 1);
        }

        // Disable ECO mode when AC mains power is restored.
        if (Power == 1 && Manag_index[1] > 0)
        {
            Manag_index[1] = 0;

            // Synchronize the new state with the database.
            Message_DB(-2, 0, 1);
        }
    }

    // Lighting scenario executed when ECO mode becomes active.
    if (Manag_index[1] == 1)
    {
        // Ensure that the scenario is executed only once.
        Manag_index[1] = 2;

        // Iterate through all lighting zones.
        for (int i = 1; i < 8; i++)
        {
            // If the room lighting is currently ON
            // (a pending OFF command is stored in the state machine),
            // briefly toggle the lighting configuration.
            if (kit[i] == -1)
            {
                // Temporarily invert the pending command.
                kit[i] = 1;

                // Reconfigure the lighting.
                Main_Switch(i);

                // Restore the pending OFF command.
                kit[i] = -1;
            }
        }
    }

    // Notify about AC mains power state changes.
    if (Power == 1 && Power_TB == 0)
    {
        Power_TB = 1;

        // Send a Telegram notification.
        Telebot_message(1, 12);

        // Update the database.
        Message_DB(-1, 1, 12);
    }

    if (Power == 0 && Power_TB == 1)
    {
        Power_TB = 0;

        // Send a Telegram notification.
        Telebot_message(1, 12);

        // Update the database.
        Message_DB(-1, 0, 12);
    }
}