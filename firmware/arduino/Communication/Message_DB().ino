// Build and transmit SQL update commands to the database gateway.
void Message_DB(int8_t Zone, uint8_t State, uint8_t id)
{
    // Update Manual Switch Panel (ON).
    if (Zone == 0 && State == 1)
    {
        String Message_ON_msp;
        Message_ON_msp.reserve(60);

        Message_ON_msp += "mariadb s_h -e \"UPDATE msp SET state='on' WHERE id=";
        Message_ON_msp += id;
        Message_ON_msp += ";\"";

        Serial1.println(Message_ON_msp);
        delay(Delay_S1);
    }

    // Update Manual Switch Panel (OFF).
    if (Zone == 0 && State == 0)
    {
        String Message_OFF_msp;
        Message_OFF_msp.reserve(60);

        Message_OFF_msp += "mariadb s_h -e \"UPDATE msp SET state='off' WHERE id=";
        Message_OFF_msp += id;
        Message_OFF_msp += ";\"";

        Serial1.println(Message_OFF_msp);
        delay(Delay_S1);
    }

    // Update Local Lighting Switches (ON).
    if (Zone > 0 && State > 0)
    {
        String Message_ON_lls;
        Message_ON_lls.reserve(60);

        Message_ON_lls += "mariadb s_h -e \"UPDATE lls SET state='on' WHERE id=";
        Message_ON_lls += Zone;
        Message_ON_lls += ";\"";

        Serial1.println(Message_ON_lls);
        delay(Delay_S1);
    }

    // Update Local Lighting Switches (OFF).
    if (Zone > 0 && State == 0)
    {
        String Message_OFF_lls;
        Message_OFF_lls.reserve(60);

        Message_OFF_lls += "mariadb s_h -e \"UPDATE lls SET state='off' WHERE id=";
        Message_OFF_lls += Zone;
        Message_OFF_lls += ";\"";

        Serial1.println(Message_OFF_lls);
        delay(Delay_S1);
    }
        // Update Alarm Settings table.
    if (Zone == -1)
    {
        // Set switch state to ON.
        if (State == 1)
        {
            String Message_ON_a_s;
            Message_ON_a_s.reserve(60);

            Message_ON_a_s += "mariadb s_h -e \"UPDATE a_s SET state='on' WHERE id=";
            Message_ON_a_s += id;
            Message_ON_a_s += ";\"";

            Serial1.println(Message_ON_a_s);
            delay(Delay_S1);
        }

        // Set switch state to OFF.
        if (State == 0)
        {
            String Message_OFF_a_s;
            Message_OFF_a_s.reserve(60);

            Message_OFF_a_s += "mariadb s_h -e \"UPDATE a_s SET state='off' WHERE id=";
            Message_OFF_a_s += id;
            Message_OFF_a_s += ";\"";

            Serial1.println(Message_OFF_a_s);
            delay(Delay_S1);
        }
    }
    // Update Management Switches table.
    if (Zone == -2)
    {
        // Set switch state to ON.
        if (State == 1)
        {
            String Message_ON_m_s;
            Message_ON_m_s.reserve(60);

            Message_ON_m_s += "mariadb s_h -e \"UPDATE m_s SET state='on' WHERE id=";
            Message_ON_m_s += id;
            Message_ON_m_s += ";\"";

            Serial1.println(Message_ON_m_s);
            delay(Delay_S1);
        }
                // Set switch state to OFF.
        if (State == 0)
        {
            String Message_OFF_m_s;
            Message_OFF_m_s.reserve(60);

            Message_OFF_m_s += "mariadb s_h -e \"UPDATE m_s SET state='off' WHERE id=";
            Message_OFF_m_s += id;
            Message_OFF_m_s += ";\"";

            Serial1.println(Message_OFF_m_s);
            delay(Delay_S1);

            // Serial.println(Message_OFF_m_s); // Debug output.
        }
    }

    // Update Blackout Switches table.
    if (Zone == -3)
    {
        // Set switch state to ON.
        if (State == 1)
        {
            String Message_ON_b_o;
            Message_ON_b_o.reserve(60);

            Message_ON_b_o += "mariadb s_h -e \"UPDATE b_o SET state='on' WHERE id=";
            Message_ON_b_o += id;
            Message_ON_b_o += ";\"";

            Serial1.println(Message_ON_b_o);
            delay(Delay_S1);
        }

        // Set switch state to OFF.
        if (State == 0)
        {
            String Message_OFF_b_o;
            Message_OFF_b_o.reserve(60);

            Message_OFF_b_o += "mariadb s_h -e \"UPDATE b_o SET state='off' WHERE id=";
            Message_OFF_b_o += id;
            Message_OFF_b_o += ";\"";

            Serial1.println(Message_OFF_b_o);
            delay(Delay_S1);
        }
    }

    // Update Power Metering table (integer values).
    if (Zone == -4)
    {
        String Message_to_p_m;
        Message_to_p_m.reserve(60);

        Message_to_p_m += "mariadb s_h -e \"UPDATE p_m SET value='";
        Message_to_p_m += State;
        Message_to_p_m += "' WHERE id=";
        Message_to_p_m += id;
        Message_to_p_m += ";\"";

        Serial1.println(Message_to_p_m);
        delay(Delay_S1);
    }
}