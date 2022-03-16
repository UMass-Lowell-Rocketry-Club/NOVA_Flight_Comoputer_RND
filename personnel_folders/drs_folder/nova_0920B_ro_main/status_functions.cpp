#include "macros.hh"
#include "Vector3D.hh"
/*
File contains code for functions which either set the rocket into a specific status, or detect if some external events have triggered.
*/

//This function is meant to take sensor data and then calculate if the order to launch was given, regardless of if a launch occured or not.
bool get_launch_directive_status(ICM_20948 *sensor){
    myICM.getAGMT();
    detect = sensor->accZ();
    if (detect >= z_axis_acceleration_indicating_launch){
        flstate = 2; leds[0] = CRGB::Purple; FastLED.show();
        launch_time_in_milliseconds = millis();  
        return true;
    }
    return false;
}

//This function is meant to take sensor data and then calculate if the order to launch was given, but the engine did not ignite.
bool determine_if_scrubed(){

}

void computer_shutdown_directive(){
    while(true);
}
