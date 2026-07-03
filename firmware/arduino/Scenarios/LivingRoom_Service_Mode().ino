// Enter/exit the living room lighting preset selection mode.
void LivingRoom_Service_Mode()
{
    // Toggle the service mode.
    if (LR_Serv == 0)
        LR_Serv = 1;
    else
        LR_Serv = 0;

    // Visual confirmation:
    // sequentially flash all four lighting tracks.
    byte Pause = 150;

    digitalWrite(out_52, HIGH);
    delay(Pause);

    digitalWrite(out_53, HIGH);
    digitalWrite(out_52, LOW);
    delay(Pause);

    digitalWrite(out_54, HIGH);
    digitalWrite(out_53, LOW);
    delay(Pause);

    digitalWrite(out_55, HIGH);
    digitalWrite(out_54, LOW);
    delay(Pause);

    digitalWrite(out_55, LOW);
}