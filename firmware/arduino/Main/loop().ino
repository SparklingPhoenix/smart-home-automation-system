//------------------------------------------------------
// Main scheduler
// Called continuously by Arduino framework
//------------------------------------------------------

void loop()
{
    Mechanical_switchers();                              // Process physical wall switches
    Timer_BathRoom_Vent(SetPoint_BathRoom_Vent);         // Bathroom ventilation timer
    Timer_Bedroom_Wardrobe_Vent(SetPoint_BedRoom_Vent);  // Bedroom/Wardrobe ventilation timer
    GSM_modem_Conf();                                    // GSM modem state machine
    GSM_modem_Receive_UART();                            // Receive data from GSM modem
    OrangePi_Receive_UART();                             // Receive commands from Orange Pi
    Amplifier_Receive_UART();                            // Receive data from audio amplifier
    Security_Check();                                    // Security system monitoring
    Power_Auditing();                                    // Power supply monitoring
    dht22();                                             // Read DHT22 sensors
    aht_20();                                            // Read AHT20 sensor
    Thermostat();                                        // Heating and A/C control
    Serial_To_Print();                                   // Debug output to Serial Monitor
}