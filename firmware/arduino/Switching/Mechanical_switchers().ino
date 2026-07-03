//======================================================
// Mechanical switch handler
//
// Features:
// - Software debouncing
// - Short press detection
// - Long press detection
// - Automatic lighting mode cycling
// - Delayed automatic switch-off
//======================================================

//======================================================
// Process physical wall switches
//======================================================

void Mechanical_switchers()
{
if (Init_flag==0)                                        // Wait until the startup delay has elapsed
   {
   if (Start_Init_pause==0)
      Start_Init_pause = millis();                       // Start the initialization timer

   if ((millis() - Start_Init_pause) >= Init_pause)      // Startup delay expired
      Init_flag=1;                                       // Enable switch polling
   }                                                     // Prevents false triggering caused by input stabilization

for (int i=1; i<8; i++)                                  // Scan all configured wall switches
   {
   debouncer[i].update();                                // Update debounce filter

   if (Init_flag==1)                                     // Start processing switches after initialization
     {
     if (debouncer[i].read()==0 && flag[i]==0)           // Button pressed
        {
        flag[i]=1;                                       // Store button state
        Serial.println(i);                               // Send switch number to the serial monitor
        start_point[i] = millis();                       // Record button press timestamp
        }

     if (debouncer[i].read()==0 && flag[i]==1)           // Button is still pressed
        {
        end_point[i] = millis();                         // Update current timestamp

        if (end_point[i]-start_point[i]>=time_exp)       // Long press detected
           {
           Secondary_Switch(i);                          // Execute the secondary function
        // Serial.println("Service command");
           flag[i]=2;                                    // Mark long-press event as processed
           }
        }
     if (debouncer[i].read()==1)                        // Button released
        {
        if (flag[i]==2)
           flag[i]=0;                                  // Reset long-press state

        if (flag[i]==1)                                // Short press detected
           {
           flag[i]=0;                                  // Clear button state

           if (i==1 || i==6)                           // Hallway or kitchen (3 lighting combinations)
              {
              if (kit[i]<8)
                 kit[i]++;                             // Select next lighting scene
              else
                 kit[i]=0;                             // Wrap to the first scene
              }

           if (i==2 || i==3)                           // Bedroom or wardrobe (2 lighting combinations)
              {
              if (kit[i]<4)
                 kit[i]++;
              else
                 kit[i]=0;
              }

           if (i==4)                                   // Bathroom
              {
              if (kit[i]<6)
                 kit[i]++;
              else
                 kit[i]=0;
              }

           if (i==5)                                   // Living room
              {
              if (LR_Serv==0)
                 {
                 if (kit[i]<2)
                    kit[i]++;                          // Cycle through standard lighting scenes
                 else
                    kit[i]=0;
                 }

              if (LR_Serv==1)
                 {
                 if (kit[i]<15)
                    kit[i]++;                          // Cycle through service lighting scenes
                 else
                    kit[i]=0;
                 }
              }

           if (i==7)                                   // Home office (single lighting circuit)
              {
              if (kit[i]<1)
                 kit[i]++;
              else
                 kit[i]=0;
              }

           Main_Switch(i);                             // Apply the selected lighting scene
           }
        }

     if (kit[i]>0)                                     // Any lighting scene is active
        {
        control_point[i] = millis();                   // Update inactivity timer

        if (control_point[i]-start_point[i]>=time_set) // Auto-off timeout expired
           {
           kit[i]= -1;                                 // Schedule the "All Off" scene
        // Serial.println("OFF");
           }
        }

     }
   }
}