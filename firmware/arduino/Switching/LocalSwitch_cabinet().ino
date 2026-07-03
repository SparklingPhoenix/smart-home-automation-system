// Cabinet lighting control.
void LocalSwitch_cabinet(int kit)
{
    // Enable lighting.
    if (kit == 1)
    {
        digitalWrite(out_71, HIGH);
        Message_DB(0, 1, 22);          // Update Manual Control Panel.
    }

    // Disable lighting.
    else
    {
        digitalWrite(out_71, LOW);
        Message_DB(0, 0, 22);          // Update Manual Control Panel.
    }
}