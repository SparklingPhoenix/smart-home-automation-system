// PROCESS THERMOSTAT SETPOINTS FROM THE MANAGEMENT PANEL
void Thermostat_values(byte index, float value)
{
    int adress = 0;

    // Heating thermostat setpoint updated.
    if (index == 1)
    {
        // Reset thermostat performance statistics.
        Cycle_B = 0;      // Boiler operating cycles.
        Cycle_N = 0;      // Temperature sampling counter.
        T_aver = 0;       // Average control deviation.

        adress = 105;
    }

    // Cooling thermostat setpoint updated.
    if (index == 2)
        adress = 107;

    // Heating thermostat ECO mode setpoint updated.
    if (index == 6)
        adress = 109;

    // Cooling thermostat ECO mode setpoint updated.
    if (index == 7)
        adress = 111;

    // Store the setpoint as an integer with one decimal place precision.
    int Val = value * 10;

    Therm[index] = Val;

    // Save the new setpoint to EEPROM.
    EEPROM.put(adress, Therm[index]);

    // Convert the value back to a string for database synchronization.
    String Value(value, 1);

    String Message_to_p_m;
    Message_to_p_m.reserve(60);

    Message_to_p_m += "mariadb s_h -e \"UPDATE p_m SET value=\'";
    Message_to_p_m += Value;
    Message_to_p_m += "\' WHERE id=";
    Message_to_p_m += index + 9;
    Message_to_p_m += ";\"";

    // Send the updated value to the database.
    Serial1.println(Message_to_p_m);

    // Release the allocated string buffer.
    Message_to_p_m = "";
}