//======================================================
// Process commands from the manual virtual control panel
//======================================================
void Manual_Switch_Panel(byte Array_cell)
{
   // Output pin mapping for the manual control panel
   int MSP[23] =
   {
      0,
      out_11, out_12, out_13,
      out_21, out_22, out_23,
      out_31, out_32, out_23,
      out_41, out_42, out_43, out_44,
      out_51, out_52, out_53, out_54, out_55,
      out_61, out_62, out_63,
      out_71
   };

   byte Pin = 0;
   Pin = MSP[Array_cell];

   byte Zone;

   // Determine the corresponding lighting zone
   if (0<Array_cell && Array_cell<4) Zone=1;             // Hallway
   if (3<Array_cell && Array_cell<6) Zone=2;             // Bedroom
   if (6<Array_cell && Array_cell<9) Zone=3;             // Wardrobe
   if (9<Array_cell && Array_cell<13) Zone=4;            // Bathroom
   if (13<Array_cell && Array_cell<19) Zone=5;           // Living room
   if (18<Array_cell && Array_cell<22) Zone=6;           // Kitchen
   if (Array_cell==22) Zone=7;                           // Home office

   // Special functions
   if (Array_cell==13)
      Bathroom_Vent();

   else if (Array_cell==6 || Array_cell==9)
      Bedroom_Wardrobe_Vent();

   else
   {
      if (digitalRead(Pin)==LOW)
      {
         digitalWrite(Pin, HIGH);

         Message_DB(0, 1, Array_cell);          // Update manual control panel
         delay(30);
         Message_DB(Zone, 1, 0);                // Synchronize universal control panel

         kit[Zone]=-1;
      }

      else
      {
         digitalWrite(Pin, LOW);

         Message_DB(0, 0, Array_cell);          // Update manual control panel

         // Synchronize the universal panel when all outputs in the zone are OFF
         if (digitalRead(MSP[1])==0 && digitalRead(MSP[2])==0 && digitalRead(MSP[3])==0)
            Message_DB(1, 0, 0);

         if (digitalRead(MSP[4])==0 && digitalRead(MSP[5])==0 && digitalRead(MSP[6])==0)
            Message_DB(2, 0, 0);

         if (digitalRead(MSP[7])==0 && digitalRead(MSP[8])==0 && digitalRead(MSP[9])==0)
            Message_DB(3, 0, 0);

         if (digitalRead(MSP[10])==0 && digitalRead(MSP[11])==0 && digitalRead(MSP[12])==0)
            Message_DB(4, 0, 0);

         if (digitalRead(MSP[14])==0 && digitalRead(MSP[15])==0 &&
             digitalRead(MSP[16])==0 && digitalRead(MSP[17])==0 &&
             digitalRead(MSP[18])==0)
            Message_DB(5, 0, 0);

         if (digitalRead(MSP[19])==0 && digitalRead(MSP[20])==0 &&
             digitalRead(MSP[21])==0)
            Message_DB(6, 0, 0);

         if (digitalRead(MSP[22])==0)
            Message_DB(7, 0, 0);

         kit[Zone]=0;
      }
   }
}