#include "macros.h"
#include "Vector3D.h"
void setup() {
        bool computer_active = true;
         //put your setup code here, to run once:
        FastLED.addLeds<NUM_STRIPS,CHIPSET,DATA_PIN,COLOR_ORDER>(leds,NUM_LEDS).setCorrection(TypicalLEDStrip);
        FastLED.setBrightness(Bright);
        FastLED.clear();
        FastLED.show();
        for (int i=0; i<25 ; i++){
            fill_rainbow( &(leds[0]), NUM_LEDS, i*50); FastLED.show(); delay(100);
        }
        leds[0]=CRGB::Black; FastLED.show();
        delay(500);
        pinMode(btn, INPUT);



        if (!SD.begin(10))
        {
            for (int i=0; i<5; i++)
            {
            leds[0] = CRGB::Cyan; FastLED.show(); delay(LNG_FLASH);
            leds[0] = CRGB::Red;  FastLED.show(); delay(SRT_FLASH);
            }
            while(1); //Devon: Why is this here? From what I can tell this is an infinite loop, I am not even sure how the code would run with this. 
        }
        else
        {
            for (int i=0; i<3; i++)
            {
            leds[0] = CRGB::Cyan; FastLED.show(); delay(SRT_FLASH);
            leds[0] = CRGB::Black;  FastLED.show(); delay(SRT_FLASH);
            }  
        }
        card = SD.open(data_log_file_name, FILE_WRITE);
        if (card)
        {
            card.println("Flight start up");
        }
        else
        {
            for (int i=0; i<5; i++)
            {
            leds[0] = CRGB::Cyan; FastLED.show(); delay(LNG_FLASH);
            leds[0] = CRGB::Orange; FastLED.show(); delay(LNG_FLASH);
            leds[0] = CRGB::Red;  FastLED.show(); delay(SRT_FLASH);
            }  
        }
        
        WIRE_PORT.begin();
        WIRE_PORT.setClock(400000);

        bool initialized = false;
        while(!initialized)
        {
            myICM.begin( WIRE_PORT, AD0_VAL);
            if(myICM.status != ICM_20948_Stat_Ok)
            {
            for (int i=0; i<5; i++)
                {
                leds[0] = CRGB::Green; FastLED.show(); delay(LNG_FLASH);
                leds[0] = CRGB::Red;  FastLED.show(); delay(SRT_FLASH);
                }  
            }
            else
            {
            initialized = true;
                for (int i=0; i<3; i++)
                {
                leds[0] = CRGB::Green; FastLED.show(); delay(SRT_FLASH);
                leds[0] = CRGB::Black; FastLED.show(); delay(SRT_FLASH);
                } 
                card.println("IMU startup"); 
            }
        }
        myICM.swReset();            //reseting IMU to a known state
        if (myICM.status != ICM_20948_Stat_Ok)
        {}
        delay(150);
        myICM.sleep(false);
        myICM.lowPower(false);
        myICM.setSampleMode((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), ICM_20948_Sample_Mode_Continuous);  //setting sample mode
        if (myICM.status != ICM_20948_Stat_Ok)
        {
            for (int i=0; i<5; i++)
                {
                leds[0] = CRGB::Green; FastLED.show(); delay(LNG_FLASH);
                leds[0] = CRGB::Orange; FastLED.show(); delay(SRT_FLASH);
                leds[0] = CRGB::Red;  FastLED.show(); delay(SRT_FLASH);
                }   
        }
        else
        {
            card.println("IMU reset and sample mode set");
        }
        
        ICM_20948_fss_t myFSS;
        myFSS.a = gpm16;
        myFSS.g = dps2000;
        myICM.setFullScale((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), myFSS);       //Setting data recording values
        if (myICM.status != ICM_20948_Stat_Ok)
        {
            for (int i=0; i<5; i++)
                {
                leds[0] = CRGB::Green; FastLED.show(); delay(LNG_FLASH);
                leds[0] = CRGB::Blue; FastLED.show(); delay(SRT_FLASH);
                leds[0] = CRGB::Red;  FastLED.show(); delay(SRT_FLASH);
                }   
        }
        else
        {
            card.println("IMU data value limits set +- 16g and +-2000dps");
        }
        
        while (!bme.begin())
        {
                leds[0] = CRGB::Blue; FastLED.show(); delay(SRT_FLASH);
                leds[0] = CRGB::Red;  FastLED.show(); delay(SRT_FLASH);
        }
            for (int i=0; i<3; i++)
                {
                leds[0] = CRGB::Blue; FastLED.show(); delay(SRT_FLASH);
                leds[0] = CRGB::Black;  FastLED.show(); delay(SRT_FLASH);
                } 
                card.println("BME280 started");
                leds[0] = CRGB::Yellow; FastLED.show();
        
        
        card.close();

}



/*
Code executed during main flight of the rocket is below
*/

void loop() {
    bool ignition_order_recieved = false;
    bool scrub_orders_recieved = false;
    bool flight_engaged = false;
    Vector3D velocity_vector;
    int ticks_since_no_vertical_movement = 0;
    int flight_computer_begin_tick_time = 0;
    int flight_computer_after_tick_time = 0;
    
    while(computer_active){ //Computer Active is declared in setup.
        ignition_order_recieved = get_launch_directive_status();

        //This can be dealt with later, if we have a more sophisticated launch system, until then leave it commented so we don't forget we have the capability to do it. 
        // while(ignition_order_recieved && !scrub_orders_recieved){
        //     scrub_orders_recieved = ignition_order_recieved; //determine_if_scrubed(); <- If we ever make a more sophisticated computer, we can use this to do various things like write to the log that the engine did not ignite
        // }
        if(ignition_order_recieved){
            flight_engaged = ignition_order_recieved; //Currently this is pointless, in the future though it will not be. 
        }
        while(flight_engaged){

            
            update_met(); flight_computer_begin_tick_time = MET; //Recall that MET is in the macro, this updates it and then sets the time to the MET. 
            //Dereferances myICM and preforms member selection to get acceleration. Using it this way because that is how it is down in the old CFL function I deleted.
            velocity_vector.set_x_cord(velocity_vector.get_x_cord()+ get_adjusted_acceleration((&myICM)->accX()) );
            velocity_vector.set_y_cord(velocity_vector.get_y_cord()+ get_adjusted_acceleration((&myICM)->accY()) );
            velocity_vector.set_z_cord(velocity_vector.get_z_cord()+ get_adjusted_acceleration((&myICM)->accZ()) );
            stabilize_rocket(velocity_vector); //Currently does nothing
            if(velocity_vector.get_z_cord() < 0.1 && velocity_vector.get_z_cord() > -0.1){ //0.1 m/s seems reasonable. 
                time_since_no_vertical_movement++;
                if(ticks_since_no_vertical_movement > 1000){ //assuming 30 ticks or so per second, this should be about 33 seconds
                    computer_active = false; //If no verticle movement has been detected for this long, were probably on the ground. So we turn off the computer
                }
            }
            else{
                time_since_no_vertical_movement = 0; //Clearly, something has moved the rocket, so we should keep recording data. 
            }

            /*
            Everything above this comment was dedicated to collecting data and preforming rocket functions based on that data.
            everything below this comment was dedicated to storing that data and transmiting that data. 
            */ 


            write_data_to_log(&myICM, velocity_vector); //I put here before updating the MET only because the data was calculated based on the previous met value
            update_met(); flight_computer_after_tick_time = MET; //Recall that MET is in the macro folder, this updates it and then sets the time to the MET. 
            //We check if the computation time of the flight computer up to now since the last tick has exceeded what we consider ideal, if it has we only send a partial 
            //data packet, if it has not we send a full data packet. 
            if( ( flight_computer_after_tick_time - flight_computer_begin_tick_time ) > IDEAL_TICK_COMPUTATION_TIME_IN_MILLIS){ 
                transmit_data_to_ground_partial_packet();
            }
            else{
                transmit_data_to_ground_full_packet();
            }
        }

    }


/*
Disregard everything beneath here, just havn't deleted these get because I may need them later. 
*/










      //put your main code here, to run repeatedly:
    // while (flstate == 0)
    // {
    //     if (digitalRead(btn) == true)
    //     {
    //       delay(100);
    //       if(digitalRead(btn) == true)
    //       {
    //         flstate = 1;
    //         for (int i=0; i<100; i++)
    //         {
    //         leds[0] = CRGB::Orange; FastLED.show(); delay(SRT_FLASH);
    //         leds[0] = CRGB::Red;  FastLED.show(); delay(SRT_FLASH);
    //         }
    //         leds[0] = CRGB::Black; FastLED.show();
    //       }
    //     }
        
    //   }
    //   while(flstate == 1)
    //   {
    //     CFL(&myICM);
    //     //check for launch
    //   }
    //   while(flstate == 2)
    //   {
    //     datalog(&myICM);
    //   }
    //   while(flstate == 3)
    //   {
    //     LRdatalog(&myICM);
    //   }
    //   while (flstate == 4)
    //   {
    //     //nothing, absolutely nothing... echo             echoo                         echo......
    //   }

}


/*
Secondary Functions which are not part of setup() or loop()
*/

/*
Everything below this line is supposed to be in another file
*/


/*
This is SUPPOSED to be in the status_functions.ino file but it doesn't seem to be working properly. I assume VS Code is acting odd.
*/