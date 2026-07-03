//======================================================
// Wardrobe lighting control
//======================================================
void LocalSwitch_wardrobe(int kit)
{
   // Output pins assigned to the wardrobe lighting
   byte output_pins_set[] =
   {
      out_31,
      out_32
   };

   // Lighting presets
   //
   // Columns:
   // [0] Local lighting
   // [1] Main lighting
   byte lighting_set[][2] =
   {
      {0,0},
      {1,0},
      {1,1},
      {0,1}
   };

   // In Eco mode always select the lowest-power lighting preset
   if (Manag_index[1] > 0 && kit > 0)
      kit = 3;

   // Apply the selected lighting preset
   for (int n = 0; n < 2; n++)
   {
      byte outPin = output_pins_set[n];
      byte State  = lighting_set[kit][n];

      // Enable the 12 V power supply if required
      PowerUnit_220to12V(outPin, State);

      // Switch the relay output
      digitalWrite(outPin, State);

      // Synchronize the manual control panel
      Message_DB(0, State, n + 7);
   }
}