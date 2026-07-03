// Process data received from the DHT22 temperature and humidity sensor.
void dht22()
{
    float t = 0;
    int T = 0;

    // Start the temperature measurement timer.
    if (Start_Temp_Measure_1 == 0)
        Start_Temp_Measure_1 = millis();

    // Has the temperature measurement interval elapsed?
    if (millis() - Start_Temp_Measure_1 >= SetPoint_Temp_check_1)
    {
        // Restart the timer.
        Start_Temp_Measure_1 = 0;

        // Increment the measurement counter.
        Cycle_T++;

        // Prevent the counter from overflowing.
        if (Cycle_T == 100)
            Cycle_T = 1;

        // Read the ambient temperature.
        t = dht1.readTemperature();

        // If the sensor failed to respond, schedule a power-cycle reset.
        if (isnan(t))
        {
            dht_Restart = 1;
        }
        else
        {
            // Convert the temperature to a fixed-point integer format.
            T = 10 * t;

            // Update the value only if it has changed.
            if (t1 != T)
            {
                t1 = T;

                // Apply the calibration offset and store the corrected value.
                Temperature[1] = t1 + Therm[4];

                // Split the fixed-point value into integer and fractional parts.
                String T1(Temperature[1]);
                String T01(Temperature[1]);

                T1.remove(2);
                T01.remove(0, 2);

                // Build the database update command.
                String Message_to_p_m;
                Message_to_p_m.reserve(60);

                Message_to_p_m += "mariadb s_h -e \"UPDATE p_m SET value=\'";
                Message_to_p_m += T1;
                Message_to_p_m += ".";
                Message_to_p_m += T01;
                Message_to_p_m += "\' WHERE id=6;\"";

                // Send the updated temperature to the database.
                Serial1.println(Message_to_p_m);

                Message_to_p_m = "";
            }
        }

        // Update the humidity only if it has changed.
        if (h1 != dht1.readHumidity())
        {
            h1 = dht1.readHumidity();

            // If the sensor failed to respond, schedule a power-cycle reset.
            if (isnan(h1))
            {
                dht_Restart = 1;
            }
            else
            {
                String H1(h1, 1);

                // Build the database update command.
                String Message_to_p_m;

                Message_to_p_m += "mariadb s_h -e \"UPDATE p_m SET value=\'";
                Message_to_p_m += H1;
                Message_to_p_m += "\' WHERE id=7;\"";

                // Send the updated humidity to the database.
                Serial1.println(Message_to_p_m);

                Message_to_p_m = "";
            }
        }
    }

    // Start the second temperature measurement timer.
    if (Start_Temp_Measure_2 == 0)
        Start_Temp_Measure_2 = millis();

        // Has the second temperature measurement interval elapsed?
    if (millis() - Start_Temp_Measure_2 >= SetPoint_Temp_check_2)
    {
        // Restart the timer.
        Start_Temp_Measure_2 = 0;

        // Read the ambient temperature from the second DHT22 sensor.
        float t = dht2.readTemperature();

        // Convert the temperature to a fixed-point integer format.
        int T2 = 10 * t;

        // Update the value only if it has changed.
        if (t2 != T2)
        {
            t2 = T2;

            // If the sensor failed to respond, schedule a power-cycle reset.
            if (isnan(t2))
            {
                dht_Restart = 1;
            }
            else
            {
                // Apply the calibration offset and store the corrected value.
                Temperature[2] = t2 + Therm[5];

                // Convert the fixed-point value back to floating-point.
                float T_2 = static_cast<float>(Temperature[2]) / 10;

                // Format the temperature with one decimal place.
                String T2(T_2, 1);

                // Build the database update command.
                String Message_to_p_m;
                Message_to_p_m.reserve(60);

                Message_to_p_m += "mariadb s_h -e \"UPDATE p_m SET value=\'";
                Message_to_p_m += T2;
                Message_to_p_m += "\' WHERE id=8;\"";

                // Send the updated temperature to the database.
                Serial1.println(Message_to_p_m);

                Message_to_p_m = "";
            }
        }

        // Update the humidity only if it has changed.
        if (h2 != dht2.readHumidity())
        {
            h2 = dht2.readHumidity();

            // If the sensor failed to respond, schedule a power-cycle reset.
            if (isnan(h2))
            {
                dht_Restart = 1;
            }
            else
            {
                String H2(h2, 1);

                // Build the database update command.
                String Message_to_p_m;

                Message_to_p_m += "mariadb s_h -e \"UPDATE p_m SET value=\'";
                Message_to_p_m += H2;
                Message_to_p_m += "\' WHERE id=9;\"";

                // Send the updated humidity to the database.
                Serial1.println(Message_to_p_m);

                Message_to_p_m = "";
            }
        }
    }

    // Restart the DHT22 sensor power supply if a fault was detected.
    if (dht_Restart == 1)
    {
        // Start the restart timer.
        if (DHT_Restart_timer == 0)
        {
            DHT_Restart_timer = millis();

            // Remove power from the sensors.
            digitalWrite(VCC_dht22, LOW);
        }

        // Has the power-off interval elapsed?
        if (millis() - DHT_Restart_timer >= 4000)
        {
            // Restore sensor power.
            digitalWrite(VCC_dht22, HIGH);

            // Clear the restart request.
            dht_Restart = 0;

            // Reset the restart timer.
            DHT_Restart_timer = 0;
        }
    }
}    