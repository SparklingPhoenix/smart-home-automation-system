//======================================================
// Lighting dispatcher
// Routes lighting commands to the corresponding zone
//======================================================

void Main_Switch(int number)
{
   if (number==1) LocalSwitch_hallway(kit[number]);
   if (number==2) LocalSwitch_bedroom(kit[number]);
   if (number==3) LocalSwitch_wardrobe(kit[number]);
   if (number==4) LocalSwitch_bathroom(kit[number]);
   if (number==5) LocalSwitch_livingroom(kit[number]);
   if (number==6) LocalSwitch_kitchen(kit[number]);
   if (number==7) LocalSwitch_cabinet(kit[number]);

   if (number==8 && digitalRead(out_44)==HIGH)
      Bathroom_Vent();

   if (number==9 && digitalRead(out_23)==HIGH)
      Bedroom_Wardrobe_Vent();

   if (number==10 && Amp_state==1)
      Amplifier();

   if (number<8)
      Message_DB(number, kit[number], 0);      // Synchronize the universal control panel
}