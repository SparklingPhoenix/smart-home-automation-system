//======================================================
// Kitchen lighting control
//======================================================
void LocalSwitch_kitchen(int kit)
{
   // Output pins assigned to the kitchen lighting
   byte output_pins_set[] =
   {
      out_61,   // Local LED lighting
      out_62,   // Bar counter lighting
      out_63    // Main lighting
   };

   // Kitchen lighting scene presets
   //
   // Columns:
   // [0] Local lighting
   // [1] Bar counter
   // [2] Main lighting
   byte lighting_set[][3] =
   {
      {0,0,0},
      {1,0,0},
      {0,1,0},
      {1,1,0},
      {1,1,1},
      {0,1,1},
      {1,0,1},
      {0,0,1}
   };

   // In Eco mode only the bar counter lighting is enabled
   if (Manag_index[1] > 0 && kit > 0)
      kit = 2;

   // Apply the selected lighting scene
   for (int n = 0; n < 3; n++)
   {
      byte outPin = output_pins_set[n];
      byte State  = lighting_set[kit][n];

      // Enable the 12 V power supply if required
      PowerUnit_220to12V(outPin, State);

      // Switch the relay output
      digitalWrite(outPin, State);

      // Synchronize the manual control panel
      Message_DB(0, State, n + 19);
   }
}