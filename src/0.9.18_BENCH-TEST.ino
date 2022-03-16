/*NOVA V5 code 0.9.18

  Goal: data collection and in flight data interperitation
      8 DOF ( 3 Accel, 3 Gyro, Baro, temperature)

      Interperated data
          Velocity
          Position
          Attitude
          Roll Rate
          AGL
  Sensors: ICM-20948 BME 280
  MPU: Teensy 4.0          

BENCH TESTING
SERIAL BAUD 115200

  



*/
//libraries
#include "ICM_20948.h"      //ICM-20948 IMU Accelerometer/Gyroscope/Magnetometer
#define WIRE_PORT Wire
#define AD0_VAL 1
ICM_20948_I2C myICM;

#include <BME280I2C.h>
#include <Wire.h>  //BME-280 Barometer/Thermometer/Hydrometer
BME280I2C bme;



#include <SPI.h>            //SD card 
#include <SD.h>
File card;

#include <FastLED.h>          //indicator Light
#define NUM_LEDS 1
#define DATA_PIN 2
CRGB leds[1];
#define LED_FLASH 150
#define LNG_FLASH 500

//variables
float MET = 0;      //total mission time
float dt = 0;       //delta time between data points
float MLOT = 15000; //Mission lockout time, MAX mission elapsed time





int cntbtn = 5;                                       //pin assigned to the Continue button
bool Cntbtn = false;

float AccelX = 0; float VelX = 0; float PosX = 0;
float AccelY = 0; float VelY = 0; float PosY = 0;     // Accleration Values and there Derivations
float AccelZ = 0; float VelZ = 0; float PosZ = 0;

float GyroX = 0;
float GyroY = 0;                                      //Gyroscope Values
float GyroZ = 0;
float AttiX;
float AttiY;



float AGL;
float GLP;                                            //BaroValues
float GLT;


float FLSTATE = 0;                                      // 0 = boot  1 = stby before preflight 2 = preflight 3 = stby before wait for launch   4 = wait for launch 5 = boost   6 = Coast   7 = parachuete ejection/apogee  8 = descent 9 = Landed

  

void setup() {
Serial.begin(115200);

while(!Serial)
{
}

FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, 1);
   for (int i=0; i<25 ; i++) 
    {
    fill_rainbow( &(leds[0]), NUM_LEDS, i*50); FastLED.show(); delay(100);
    }
    leds[0]=CRGB::Black; FastLED.show();
    delay(500);

//cntbtn
pinMode(cntbtn, INPUT);

//SD card Init
 if (!SD.begin(10)) 
  {
Serial.println("Failed to start SDcard");
    while (1);

  }
  else
  {
Serial.println("SDcard start");
  }
  card = SD.open("Flight.txt", FILE_WRITE);
  if (card)
  {
    card.println("Flight start up");
    Serial.println("Flight start up");
  }
  else
  {
  Serial.println("Failed to write to SD card");
  }

//ICM_20948 startup 
  WIRE_PORT.begin();
  WIRE_PORT.setClock(400000);

//ICM Check
  bool initialized = false;
  while(!initialized)
  {
    myICM.begin( WIRE_PORT, AD0_VAL);
    if( myICM.status != ICM_20948_Stat_Ok)
    {
Serial.println("IMU failed to start");
    }
    else
    {
      initialized = true;
Serial.println("IMU start");
    }
    
  }
myICM.swReset();            //reseting IMU to a known state
if (myICM.status != ICM_20948_Stat_Ok)
{
Serial.println("IMU failed restart");
}
delay(150);
myICM.sleep(false);
myICM.lowPower(false);
myICM.setSampleMode((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), ICM_20948_Sample_Mode_Continuous);  //setting sample mode
if (myICM.status != ICM_20948_Stat_Ok)
{
Serial.println("IMU failed samplemode set");
}

ICM_20948_fss_t myFSS;
myFSS.a = gpm16;
myFSS.g = dps2000;
myICM.setFullScale((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), myFSS);       //Setting data recording values
if (myICM.status != ICM_20948_Stat_Ok)
{
  Serial.println("IMU failed setting data sizes");
}





  
//BME280
while(!bme.begin())
{
  delay(100);
}
Serial.println("BARO start");
BME280::ChipModel_BME280;




Serial.println("END BOOT -- STANDBY 1");
//end of boot
leds[0] = CRGB::Yellow; FastLED.show();
FLSTATE = 1;

}




void loop() {
  // put your main code here, to run repeatedly:

if (digitalRead(cntbtn) == HIGH && FLSTATE == 1)
{
  FLSTATE = 2;  //send to preflight
}
while( FLSTATE = 3)
{
  preflight(&myICM);
}
}




//preflight obtains andaveraged value for the vehicles attitude on the rail as well as the Ground level pressure and Ground level temperature


void preflight (ICM_20948_I2C *sensor)
{
 card.println("Initial Values");
 Serial.println("PREFLIGT -- Initial Values");


//delay and warning lights "SETP BACK -- HANDS OFF"
for (int l = 0; l<10; l++)
{
  leds[0] = CRGB::Blue;
  FastLED.show();
  delay(LNG_FLASH);
  leds[0] = CRGB::Green;
  FastLED.show();
  delay(LNG_FLASH);
  if (l>5)
  {
    leds[0] = CRGB::Blue;
    FastLED.show();
    delay(LED_FLASH);
    leds[0] = CRGB::Green;
    FastLED.show();
    delay(LED_FLASH);
  }
  
}

 leds[0] = CRGB::Black; FastLED.show();
 
 for (int i=0; i<5;i++)
 {
 myICM.getAGMT();

 float temp(NAN), hum(NAN), pres(NAN);
 BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
 BME280::PresUnit presUnit(BME280::PresUnit_Pa);
 bme.read(pres, temp, hum, tempUnit, presUnit); 
 AccelX = sensor->accX() * 0.009807;
 AccelY = sensor->accY() * 0.009807;
 AccelZ = sensor->accZ() * 0.009807;
 
 AttiX = AttiX + (atan2(AccelX, AccelZ)/2/3.141592654*360);
 AttiY = AttiY + (atan2(AccelY, AccelZ)/2/3.141592654*360);

 GLP = GLP + pres;
 GLT = GLT + temp;
 
 }

AttiX = AttiX/5;
AttiY = AttiY/5;
GLP = GLP/5;
GLT = GLT/5;

card.print("AttiX: "); card.print(AttiX); card.print("\t"); card.print("AttiY: "); card.print(AttiY); card.print("\t");
card.print("GLP: "); card.print(GLP); card.print("\t"); card.print("GLT: "); card.print(GLT); card.print("\n");

Serial.print("AttiX: "); Serial.print(AttiX); Serial.print("\t"); Serial.print("AttiY: "); Serial.print(AttiY); Serial.print("\t");
Serial.print("GLP: "); Serial.print(GLP); Serial.print("\t"); Serial.print("GLT: "); Serial.print(GLT); Serial.print("\n");

FLSTATE = 3;
leds[0] = CRGB::Yellow; FastLED.show();
Serial.println("Standyby 2");
 
}



//WaitforLaunch checks the IMU's z-axis accelerometer to detect launch by comparing it to a preset value (will be improved in the future to start recording data by signal from launch controler.

void WaitforLaunch (ICM_20948_I2C *sensor)
{

float WFL_ACCEL;
float LNCH_ACCEL = 2750;  //launch thresh-hold value in milli g "1000 = 1g"


myICM.getAGMT();
WFL_ACCEL = sensor->accX();
if ( WFL_ACCEL > LNCH_ACCEL)
{
  FLSTATE = FLSTATE + 1;
  datalog1(&myICM);
}
  
}



//datalog retreives data and uses it to calculate velocity, position, rotation, and altitude measurements while storing them in CSV format on an SD card .txt file


void datalog1 (ICM_20948 *sensor) 
{
 myICM.getAGMT();

 float temp(NAN), hum(NAN), pres(NAN);
 BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
 BME280::PresUnit presUnit(BME280::PresUnit_Pa);
 bme.read(pres, temp, hum, tempUnit, presUnit);
  
  dt = 0;
  MET = MET + dt;    //time calculation

  AccelX = sensor->accX() * 0.009807; VelX = VelX + (AccelX * dt); PosX = PosX + (VelX * dt);
  AccelY = sensor->accY() * 0.009807; VelY = VelY + (AccelY * dt); PosY = PosY + (VelX * dt);     //Accelerometer Calculations mG to M/s^2
  AccelZ = sensor->accZ() * 0.009807; VelZ = VelZ + (AccelZ * dt); PosZ = PosZ + (VelZ * dt);
 



  card.print(MET);  card.print(", ");  //Mission Elapsed Time
  card.print(AccelX);  card.print(", ");  card.print(AccelY); card.print(", ");  card.print(AccelZ);  card.print(", "); //Acceleration line
  card.print(VelX); card.print(", "); card.print(VelY); card.print(", "); card.print(VelZ); card.print(", "); //Velocity Lines
  card.print(PosX); card.print(", "); card.print(PosY); card.print(", "); card.print(PosZ); card.print(", "); //Position Lines
  card.print(pres); card.print(", "); card.print(temp); card.print(", ");           //BME 280 Line

  Serial.print(MET);  Serial.print(", ");  //Mission Elapsed Time
  Serial.print(AccelX);  Serial.print(", ");  Serial.print(AccelY); Serial.print(", ");  Serial.print(AccelZ);  Serial.print(", "); //Acceleration line
  Serial.print(VelX); Serial.print(", "); Serial.print(VelY); Serial.print(", "); Serial.print(VelZ); Serial.print(", "); //Velocity Lines
  Serial.print(PosX); Serial.print(", "); Serial.print(PosY); Serial.print(", "); Serial.print(PosZ); Serial.print(", "); //Position Lines
  Serial.print(pres); Serial.print(", "); Serial.print(temp); Serial.print(", ");           //BME 280 Line


  
}



void datalog (ICM_20948 *sensor) 
{
 myICM.getAGMT();

 float temp(NAN), hum(NAN), pres(NAN);
 BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
 BME280::PresUnit presUnit(BME280::PresUnit_Pa);
 bme.read(pres, temp, hum, tempUnit, presUnit);
  
  dt = millis();
  MET = MET + dt;    //time calculation

  AccelX = sensor->accX() * 0.009807; VelX = VelX + (AccelX * dt); PosX = PosX + (VelX * dt);
  AccelY = sensor->accY() * 0.009807; VelY = VelY + (AccelY * dt); PosY = PosY + (VelX * dt);     //Accelerometer Calculations mG to M/s^2
  AccelZ = sensor->accZ() * 0.009807; VelZ = VelZ + (AccelZ * dt); PosZ = PosZ + (VelZ * dt);
 



  card.print(MET);  card.print(", ");  //Mission Elapsed Time
  card.print(AccelX);  card.print(", ");  card.print(AccelY); card.print(", ");  card.print(AccelZ);  card.print(", "); //Acceleration line
  card.print(VelX); card.print(", "); card.print(VelY); card.print(", "); card.print(VelZ); card.print(", "); //Velocity Lines
  card.print(PosX); card.print(", "); card.print(PosY); card.print(", "); card.print(PosZ); card.print(", "); //Position Lines
  card.print(pres); card.print(", "); card.print(temp); card.print(", ");           //BME 280 Line


  Serial.print(MET);  Serial.print(", ");  //Mission Elapsed Time
  Serial.print(AccelX);  Serial.print(", ");  Serial.print(AccelY); Serial.print(", ");  Serial.print(AccelZ);  Serial.print(", "); //Acceleration line
  Serial.print(VelX); Serial.print(", "); Serial.print(VelY); Serial.print(", "); Serial.print(VelZ); Serial.print(", "); //Velocity Lines
  Serial.print(PosX); Serial.print(", "); Serial.print(PosY); Serial.print(", "); Serial.print(PosZ); Serial.print(", "); //Position Lines
  Serial.print(pres); Serial.print(", "); Serial.print(temp); Serial.print(", ");           //BME 280 Line



  

if ( MET < 3000 && AccelZ < 2.0)
{
  FLSTATE = 6; //Coast   MET <3000 means before 3 seconds, AccelZ < 2.0 less than 2 m/s^2 acceleration, FLSTATE == 5 prevents this line of code from running again.
}
else if (FLSTATE = 6 && AttiX < 20 or AttiY < 20 or AccelZ > 13)
{
  FLSTATE = 7;  //Ejection or Apogee   FLSTATE lockout prevents this from running uncessairly, AttiX / AttiY are tilt apogee sensing, AccelZ > 13 is ejection charge detection
}
else if (FLSTATE = 7)
{
  FLSTATE = 8;  
}
else if (MET > MLOT)
{
  FLSTATE = 9;  //Mission LockOut Time prevents data loss from power loss by ending data logging and saving the data before it ends
}
else if (FLSTATE = 8 && cntbtn == HIGH)
{
  FLSTATE = 9.1;
}
}



void DATASAVE ()
{
  if (FLSTATE == 9.1)
  {
    card.print("Flight end by manual input");
  }
  else
  {
    card.print("Mission Time Lockout");
  }
  card.close();
  leds[0] = CRGB::Purple;
  FastLED.show();
  FLSTATE = 10;
}
