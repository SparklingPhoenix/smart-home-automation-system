// Send a command to the Telegram bot.
void Telebot_message(byte Type, byte index)
{
    String Name;

    // Select the corresponding Telegram bot script.
    if (Type == 1) Name = "php ./TelBot/s_s";   // Security state: armed / alarm
    if (Type == 2) Name = "php ./TelBot/mtg";   // Energy management settings
    if (Type == 3) Name = "php ./TelBot/thrm";  // Thermostat settings
    if (Type == 4) Name = "php ./TelBot/btn";   // Lighting control
    if (Type == 5) Name = "php ./TelBot/rng";   // Doorbell notification

    // Build the command string to be sent via the serial port.
    String Message_php_Bot;
    Message_php_Bot.reserve(30);

    Message_php_Bot += Name;
    Message_php_Bot += index;
    Message_php_Bot += ".php &";

    // Send the command to OrangePi.
    Serial1.println(Message_php_Bot);

    // Print the command to the serial monitor for debugging.
    Serial.println(Message_php_Bot);

    // Allow the external script enough time to start.
    delay(Delay_S1);
}