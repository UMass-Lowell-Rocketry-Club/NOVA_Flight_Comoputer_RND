#include <FastLED.h>                                  //FastLED library
#define NUM_LEDS 1
#define DATA_PIN 9
#define NUM_STRIPS 1
#define CHIPSET WS2812B
#define COLOR_ORDER GRB
#define Bright 100
CRGB leds[NUM_LEDS];
#define SRT_FLASH 300
#define LNG_FLASH 500


#define btn 3                                         //Button

#include <SPI.h>
#include <SD.h>
File card;

#include "ICM_20948.h"
#define WIRE_PORT Wire
#define AD0_VAL 1
ICM_20948_I2C myICM;

#include <BME280I2C.h>
#include <Wire.h>
BME280I2C bme;


float dt1 = 0;
float dt2 = 0;
float d0 = 0;
float MET = 0;
float launch = 2500;
float detect = 0;
bool btnon = false;

float flstate = 0;  //0 prelauch standby, 1 wait for launch, 2 flight, 3 late stage flight, 4 end

void setup() {
  // put your setup code here, to run once:
FastLED.addLeds<NUM_STRIPS,CHIPSET,DATA_PIN,COLOR_ORDER>(leds,NUM_LEDS).setCorrection(TypicalLEDStrip);
FastLED.setBrightness(Bright);
FastLED.clear();
FastLED.show();
    for (int i=0; i<25 ; i++) 
      {
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
  while(1);
}
else
{
  for (int i=0; i<3; i++)
  {
    leds[0] = CRGB::Cyan; FastLED.show(); delay(SRT_FLASH);
    leds[0] = CRGB::Black;  FastLED.show(); delay(SRT_FLASH);
  }  
}
card = SD.open("Flight.txt", FILE_WRITE);
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

void loop() {
  // put your main code here, to run repeatedly:
while (flstate == 0)
{
  if (digitalRead(btn) == true)
  {
    delay(100);
    if(digitalRead(btn) == true)
    {
      flstate = 1;
      for (int i=0; i<100; i++)
      {
       leds[0] = CRGB::Orange; FastLED.show(); delay(SRT_FLASH);
       leds[0] = CRGB::Red;  FastLED.show(); delay(SRT_FLASH);
      }
      leds[0] = CRGB::Black; FastLED.show();
    }
  }
  
}
while(flstate == 1)
{
  CFL(&myICM);
  //check for launch
}
while(flstate == 2)
{
  datalog(&myICM);
}
while(flstate == 3)
{
  LRdatalog(&myICM);
}
while (flstate == 4)
{
  //nothing, absolutely nothing... echo             echoo                         echo......
}

}

void CFL (ICM_20948 *sensor)
{   
   myICM.getAGMT();
   detect = sensor->accZ();
   if (detect >= launch)
   {
    flstate = 2; leds[0] = CRGB::Purple; FastLED.show();
    d0 = millis();
    
   }
}

void datalog (ICM_20948 *sensor) 
{

    card = SD.open("Flight.txt", FILE_WRITE);
    myICM.getAGMT();
    float temp(NAN), hum(NAN), pres(NAN);
    BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
    BME280::PresUnit presUnit(BME280::PresUnit_Pa);
    bme.read(pres, temp, hum, tempUnit, presUnit);   
    MET = millis()-d0;
    card.print(MET); card.print(", "); 
    card.print(sensor->accX()); card.print(", ");card.print(sensor->accY()); card.print(", ");card.print(sensor->accZ()); card.print(", ");
    card.print(sensor->gyrX()); card.print(", ");card.print(sensor->gyrY()); card.print(", ");card.print(sensor->gyrZ()); card.print(", ");
    card.print(pres); card.print(", "); card.print(temp); card.print(", "); card.println(hum);
    card.close();

    if (MET > 100000)
    {
      flstate = 3;
      leds[0] = CRGB::Navy; FastLED.show();
    }

    if(digitalRead(btn) == true)
    {
      delay(100);
      if (digitalRead(btn)==true)
      {
        flstate = 4;
        leds[0] = CRGB::ForestGreen; FastLED.show();
        card.close();
      }
    }

}
void LRdatalog (ICM_20948 *sensor) 
{

    card = SD.open("Flight.txt", FILE_WRITE);
    myICM.getAGMT();
    float temp(NAN), hum(NAN), pres(NAN);
    BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
    BME280::PresUnit presUnit(BME280::PresUnit_Pa);
    bme.read(pres, temp, hum, tempUnit, presUnit);   
    MET = millis()-d0;
    card.print(MET); card.print(", "); 
    card.print(sensor->accX()); card.print(", ");card.print(sensor->accY()); card.print(", ");card.print(sensor->accZ()); card.print(", ");
    card.print(sensor->gyrX()); card.print(", ");card.print(sensor->gyrY()); card.print(", ");card.print(sensor->gyrZ()); card.print(", ");
    card.print(pres); card.print(", "); card.print(temp); card.print(", "); card.println(hum);
    card.close();
    delay(100);

    if(digitalRead(btn) == true)
    {
      delay(100);
      if (digitalRead(btn)==true)
      {
        flstate = 4;
        leds[0] = CRGB::ForestGreen; FastLED.show();
        card.close();
      }
    }

    if( MET > 420000)
    {
      flstate = 4;
      leds[0] = CRGB::ForestGreen; FastLED.show();
      card.close();
    }

}
