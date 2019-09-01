/*
cra-slider-game, 2014-2019 Musework Exhibits, LLC
created for Colorado River Alliance's Mobile River traveling exhibit.
This code runs on an Arduino Mega attached to 6 linear softpots.
This allows a user to estimate their weekly water consumption by changing the slider / softpot
values.

MIT-license
*/

#include <Arduino.h>
#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <SPI.h>            // print to serial for debug. Adafruit_GFX wants it as a dependency
#include <EEPROM.h>         // EEPROM allows us to save and load variables between sessions
#include "EEPROMAnything.h"

//define output pins for RGBmatrix
#define CLK 11 // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A11
#define OE 9
#define A A8
#define B A9
#define C A10
// define pot input
#define POTA A0
#define POTB A1
#define POTC A2
#define POTD A3
#define POTE A4
#define POTF A5

// global variables
int mappedVal = 0;
int hue = 0;
unsigned long countdown = 60000; //countdown time IN MSEC!!!
unsigned long endTime;
unsigned long startTime;

//calibration variables store raw readouts of softpots
int showerLo;
int showerHi;
int toiletLo;
int toiletHi;
int sinkLo;
int sinkHi;
int dishesLo;
int dishesHi;
int lawnLo;
int lawnHi;
int laundryLo;
int laundryHi;

struct config_t
{
  //calibration variables store raw readouts of softpots
  int showerLo;
  int showerHi;
  int toiletLo;
  int toiletHi;
  int sinkLo;
  int sinkHi;
  int dishesLo;
  int dishesHi;
  int lawnLo;
  int lawnHi;
  int laundryLo;
  int laundryHi;
};

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, true);

void setup()
{
  Serial.begin(9600);
  // load saved calibration values from eeprom
  config_t config; // = {950, 0, 940, 0, 930, 0, 900, 0, 940, 0, 930, 0};
  EEPROM.get(0, config);
  Serial.println("read from EEPROM");
  showerLo = config.showerLo;
  showerHi = config.showerHi;
  toiletLo = config.toiletLo;
  toiletHi = config.toiletHi;
  sinkLo = config.sinkLo;
  sinkHi = config.sinkHi;
  dishesLo = config.dishesLo;
  dishesHi = config.dishesHi;
  lawnLo = config.lawnLo;
  lawnHi = config.lawnHi;
}

void loop()
{
  // read analog values

  matrix.begin();

  startTime = millis();
  endTime = startTime;
  while ((endTime - startTime) <= countdown)
  {
    //do calibration



    endTime = millis();
    int seconds = (endTime - startTime) / 1000;
    Serial.println(seconds);
  }

  int showerRaw = (analogRead(POTA));
  int toiletRaw = (analogRead(POTB));
  int sinkRaw = (analogRead(POTC));
  int dishesRaw = (analogRead(POTF));
  int laundryRaw = (analogRead(POTE));
  int lawnRaw = (analogRead(POTD));

  int mappedShower = map(showerRaw, showerLo, showerHi, 0, 40);

  mappedVal = (mappedShower);
  //mappedVal= mappedWater;
  // mappedVal= water;

  hue = map(mappedVal, 0, 100, 400, 0);
  if (hue < 50)
  {
    hue = 50;
  }

  //read analog in

  //convert int into indivs
  char strIn[3];
  String tempStr;

  tempStr = String(mappedVal);   //convert int to string
  tempStr.toCharArray(strIn, 4); // pass values to char array

  char digit1 = strIn[0];
  char digit2 = strIn[1];
  char digit3 = strIn[2];

  //if(abs(mappedVal-curVal > 3)
  // fill the screen with 'black'
  matrix.fillScreen(matrix.Color333(0, 0, 0));

  // draw some text!

  matrix.setTextSize(1); // size 1 == 8 pixels high
  matrix.setTextColor(matrix.ColorHSV(hue, 255, 255, true));
  // matrix.setCursor(0,8);
  // matrix.print(mappedVal);
  //matrix.print(digit1);
  // matrix.print(digit2);
  // matrix.print(digit3);

  //matrix.fillScreen(matrix.Color333(0, 0, 0));
  //delay (100);
  if (mappedVal < 10)
  {
    matrix.setCursor(22, 0); //set right digit
    matrix.print(digit1);

    //matrix.setCursor(18, 0); //set mid digit
    //matrix.print(0);

    //matrix.setCursor(0, 0); //set left digit
    //matrix.print(0);
  }

  else if (mappedVal > 9 && mappedVal < 100)
  {

    matrix.setCursor(22, 0); //set right digit
    matrix.print(digit2);

    matrix.setCursor(16, 0); //set mid digit
    matrix.print(digit1);

    //matrix.setCursor(0, 0); //set left digit
    //matrix.print(0);
  }
  else
  {
    matrix.setCursor(22, 0); //set right digit
    matrix.print(digit3);

    matrix.setCursor(16, 0); //set mid digit
    matrix.print(digit2);

    matrix.setCursor(10, 0); //set left digit
    matrix.print(digit1);

    //curVal = mappedVal;
    //matrix.swapBuffers(true);
    //delay(1000);
  }
  //delay(250);

  matrix.setCursor(10, 9);
  matrix.print("Gal");

  matrix.swapBuffers(false);
  //curVal=mappedVal;
  //delay(1);
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  matrix.swapBuffers(false);
  //delay(1);
}
