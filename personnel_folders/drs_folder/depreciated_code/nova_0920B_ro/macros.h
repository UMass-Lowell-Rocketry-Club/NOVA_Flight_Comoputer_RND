/*
 * Include Statements
 */
#include <string>
#include <vector>
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
const double IDEAL_TICK_COMPUTATION_TIME_IN_MILLIS = 10; //idk whatever number works best. 
const double 
const double z_axis_acceleration_indicating_launch = 2500.0; 
const string data_log_file_name = "Flight.txt"; //I see that string isn't working here, idk why. 

/*
 * Global Variables
 */
File card;
CRGB leds[NUM_LEDS];
ICM_20948_I2C myICM;
BME280I2C bme;
double dt1 = 0;
double dt2 = 0;
double launch_time_in_milliseconds = 0; 
double MET = 0; 

double detect = 0;
bool btnon = false;