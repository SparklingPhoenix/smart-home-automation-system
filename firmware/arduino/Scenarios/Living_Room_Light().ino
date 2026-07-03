//======================================================
// Living room track lighting control
//======================================================
void Living_Room_Light(int kit)
{
   // Predefined track lighting scenes
   //
   // Columns:
   // [0] Track 1
   // [1] Track 2
   // [2] Track 3
   // [3] Track 4
   byte lighting_set[][4] =
   {
      {0,0,0,0},
      {1,0,0,0},
      {0,1,0,0},
      {0,0,1,0},
      {0,0,0,1},
      {1,1,0,0},
      {0,1,1,0},
      {0,0,1,1},
      {1,0,0,1},
      {1,0,1,0},
      {0,1,0,1},
      {1,1,1,0},
      {0,1,1,1},
      {1,0,1,1},
      {1,1,0,1},
      {1,1,1,1}
   };

   // In Eco mode only Track 1 remains enabled
   if (Manag_index[1] > 0 && kit > 0)
      kit = 1;

   // Store the selected lighting scene
   Track_1 = lighting_set[kit][0];
   Track_2 = lighting_set[kit][1];
   Track_3 = lighting_set[kit][2];
   Track_4 = lighting_set[kit][3];

   // Apply relay outputs
   digitalWrite(out_52, Track_1);
   digitalWrite(out_53, Track_2);
   digitalWrite(out_54, Track_3);
   digitalWrite(out_55, Track_4);

   // Synchronize the manual control panel
   Message_DB(0, Track_1, 15);
   Message_DB(0, Track_2, 16);
   Message_DB(0, Track_3, 17);
   Message_DB(0, Track_4, 18);

   // Save the current scene to EEPROM
   EEPROM.put(101, Track_1);
   EEPROM.put(102, Track_2);
   EEPROM.put(103, Track_3);
   EEPROM.put(104, Track_4);
}