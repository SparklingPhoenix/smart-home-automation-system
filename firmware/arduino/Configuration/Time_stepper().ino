// Process timer and delay settings used by automation scenarios.
void Time_stepper(int index, uint32_t value, uint8_t Rec)
{
    // Store the new value in EEPROM if requested.
    if (Rec == 1)
    {
        // Calculate the EEPROM address assigned to this parameter.
        int adress = (index * 4) + 37;

        // Save the new value to non-volatile memory.
        EEPROM.put(adress, value);

        // Print debug information.
        Serial.print("EEPROM adress: ");
        Serial.print(adress);
        Serial.print(".  EEPROM value: ");
        Serial.println(value);
    }

    // Update the corresponding value in the database.
    String Message_to_t_v;
    Message_to_t_v.reserve(60);

    Message_to_t_v += "mariadb s_h -e \"UPDATE t_v SET value=\'";
    Message_to_t_v += value;
    Message_to_t_v += "\' WHERE id=";
    Message_to_t_v += index;
    Message_to_t_v += ";\"";

    Serial1.println(Message_to_t_v);

    Message_to_t_v = "";

    // Update the corresponding runtime parameter.
    if (index == 1)  time_exp               = value * 1000;
    if (index == 2)  time_set               = value * 1000;
    if (index == 3)  SetPoint_BathRoom_Vent = value * 1000;
    if (index == 4)  SetPoint_BedRoom_Vent  = value * 1000;
    if (index == 5)  SetPoint_GirthRail     = value * 1000;
    if (index == 6)  SetPoint_DoorState     = value * 1000;
    if (index == 7)  SetPoint_DoorBell      = value;
    if (index == 8)  SetPoint_Power_check   = value * 1000;
    if (index == 9)  SetPoint_Temp_check_1  = value * 1000;
    if (index == 10) SetPoint_Temp_check_2  = value * 1000;
    if (index == 11) boiler_cycle_ON        = value * 1000;
    if (index == 12) SetPoint_DoorButton    = value * 1000;
    if (index == 13) SetPoint_AHT20         = value * 1000;
    if (index == 14) AC_cycle_CYCLE         = value * 1000;
    if (index == 15) AC_cycle_MIN           = value * 1000;
    if (index == 16) delayed_start_AC       = value * 1000;
}