//======================================================
// Process commands from the universal virtual control panel
//======================================================
void Universal_Switch_Panel(int Zone)
{
   // Lighting zones
   if (Zone>=0 && Zone<=7)
      {
      if (kit[Zone]!=0)
         {
         kit[Zone]=0;                         // Turn lighting off
         Main_Switch(Zone);
         }
      else
         {
         kit[Zone]++;                         // Activate the default lighting scene
         Main_Switch(Zone);
         }
      }

   if (Zone==8)
      Bathroom_Vent();                        // Toggle bathroom ventilation

   if (Zone==9)
      Bedroom_Wardrobe_Vent();                // Toggle bedroom/wardrobe ventilation

   if (Zone==10)
      Amplifier();                            // Toggle audio amplifier

   if (Zone==11)
      Bluetooth_Amp();                        // Toggle Bluetooth receiver

   if (Zone==12)
      BlackOut();                             // Execute BlackOut scenario

   if (Zone==13)
      {
      if (boiler_state==0)
         boiler=1;
      else
         boiler=0;

      Manag_index[4]=0;                       // Disable thermostat mode
      Message_DB(-2, 0, 4);                   // Synchronize Management Panel
      }

// if (Zone==14)
//    AirConditioner();                       // Reserved for future implementation
}