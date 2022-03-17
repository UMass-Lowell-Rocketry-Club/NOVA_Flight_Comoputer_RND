#ifndef MACROS
#define MACROS
/*
 * Include Statements
 */
#include <string>
#include <vector>
//If you are having issues with the include statements and just want to code in c++ without dealing with (most) of the arduino libraries
//comment everything beneat this line until you see the next comment with the appropriate mention. 


#include <FastLED.h>                                  //FastLED library
#include "ICM_20948.h"
#include <SPI.h>
#include <SD.h>
#include "ICM_20948.h"
#include <BME280I2C.h>
#include <Wire.h>

/*
 * Macro Defintions and consts
 */
#define NUM_LEDS 1
#define DATA_PIN 9
#define NUM_STRIPS 1
#define CHIPSET WS2812B
#define COLOR_ORDER GRB
#define Bright 100
#define SRT_FLASH 300
#define LNG_FLASH 500
#define btn 3                                         //Button
#define WIRE_PORT Wire
#define AD0_VAL 1
#define I32_SIZE sizeof(signed int) * 8
const double IDEAL_TICK_COMPUTATION_TIME_IN_MILLIS = 10; //idk whatever number works best. 
const double 
const double z_axis_acceleration_indicating_launch = 2500.0; 
const string data_log_file_name = "Flight.txt"; //I see that string isn't working here, idk why. 
const int num_of_command_functions = 20;
std::vector<void (*)()> array_of_addresses_of_command_functions;

/*
 * Global Variables
 */
File card;
CRGB leds[NUM_LEDS];
ICM_20948_I2C myICM;
BME280I2C bme;



//End commenting out stuff here
double dt1 = 0;
double dt2 = 0;
double launch_time_in_milliseconds = 0; 
double MET = 0; 

double detect = 0;
bool btnon = false;
bool commanded_to_send_less_data = false;

#endif