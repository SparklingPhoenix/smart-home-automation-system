//======================================================
// Bathroom lighting control
//======================================================
void LocalSwitch_bathroom(int kit)
{
   // Output pins assigned to the bathroom lighting
   byte output_pins_set[] =
   {
      out_41,
      out_42,
      out_43
   };

   // Lighting presets
   //
   // Columns:
   // [0] Local lighting
   // [1] Main lighting
   // [2] Mirror lighting
   byte lighting_set[][3] =
   {
      {0,0,0},
      {1,0,1},
      {1,1,1},
      {0,1,1},
      {1,1,0},
      {0,0,1}
   };

   // Eco mode is intentionally disabled for this zone.

   // Apply the selected lighting preset
   for (int n = 0; n < 3; n++)
   {
      byte outPin = output_pins_set[n];
      byte State  = lighting_set[kit][n];

      // Enable the 12 V power supply if required
      PowerUnit_220to12V(outPin, State);

      // Switch the relay output
      digitalWrite(outPin, State);

      // Synchronize the manual control panel
      Message_DB(0, State, n + 10);
   }
}