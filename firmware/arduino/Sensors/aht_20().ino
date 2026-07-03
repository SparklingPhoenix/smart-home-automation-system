// Process data received from the AHT20 temperature and humidity sensor.
// (12 V power supply monitoring)
void aht_20()
{
    // Start the periodic measurement timer.
    if (Start_label_AHT20 == 0)
        Start_label_AHT20 = millis();

    // Update the current timestamp.
    Current_label_AHT20 = millis();

    // Has the measurement interval elapsed?
    if (Current_label_AHT20 - Start_label_AHT20 >= SetPoint_AHT20)
    {
        // Restart the timer.
        Start_label_AHT20 = 0;

        // Read the sensor if a new measurement is available.
        if (aht20.startMeasurementReady(/* crcEn = */ true))
        {
            // Read temperature and relative humidity.
            float Temp = aht20.getTemperature_C();
            float Humid = aht20.getHumidity_RH();

            // Overtemperature protection for the 12 V power supply.
            if (Temp > T_PowerUnit)
            {
                // Block the power supply from being enabled.
                PowerUnit_STOP = 1;

                // Emergency shutdown if the power supply is currently active.
                if (digitalRead(PowerUnit_12V) == HIGH)
                    digitalWrite(PowerUnit_12V, LOW);
            }
            else
            {
                // Temperature is within the safe operating range.
                PowerUnit_STOP = 0;
            }

            // Round the temperature to one decimal place.
            float T = round(Temp * 10.0) / 10.0;

            // Convert the temperature to a string.
            String temp(T);

            // Remove the last character from the formatted string.
            temp.remove(4);

            // Build and send database update commands.
            String Message_to_p_m;
            Message_to_p_m.reserve(60);

            // Update the temperature value.
            Message_to_p_m += "mariadb s_h -e \"UPDATE p_m SET value=\'";
            Message_to_p_m += temp;
            Message_to_p_m += "\' WHERE id=18;\"";
            Serial1.println(Message_to_p_m);

            Message_to_p_m = "";

            // Update the humidity value.
            Message_to_p_m += "mariadb s_h -e \"UPDATE p_m SET value=\'";
            Message_to_p_m += Humid;
            Message_to_p_m += "\' WHERE id=19;\"";
            Serial1.println(Message_to_p_m);

            Message_to_p_m = "";
        }
    }
}