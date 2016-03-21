
// Const settings for LED 

int delaymin =15;
int delaymax =80;
int brightMin= 200;

// normal on weightings
int blueVal = 0;
int greenVal = 254;
int redVal = 0;

// clash color weightings
int blueCVal = 255;
int greenCVal= 255;
int redCVal = 255;

// length of clash visuals
const int CLASHDUR = 200;   // length of clash in millis
const int CLASHMIN = 150;   // min brightness of clash

#include "Settings.h"

// global variables for storing info
long int lengthTime;
int target;
int last;
long int timetrigger;
bool clashing = false;
long int clashStop;


// number of clashes detected, usefull for battle progression
int clashCount = 0;

// sets the normal and clash color randomly
void randomColor()
{
  blueVal = random(0,255);
  greenVal = random(0,255);
  redVal = random(0,255);

  blueCVal = random(0,255);
  greenCVal = random(0,255);
  redCVal = random(0,255);
  
}

// updates LED based on current settings
void updatelight(){
  long int currenttime = millis();

    // need to update light and set new light target
    if (currenttime >= timetrigger) {
      int brightness;
      if (clashing){
        if (currenttime > clashStop){
          clashing = false;
        }
        lengthTime = CLASHDUR/4;
        brightness = random(CLASHMIN, 255);
      }
      else{
        lengthTime = random(delaymin,delaymax);
        brightness = random(brightMin, 255);
      }
      timetrigger = millis()+lengthTime;
      last =target;
      target=brightness;
    }
    
    // linear extrap of current brightness, no reset of target
    else {
      int scale = target - float(timetrigger-currenttime)/(lengthTime)*(target-last);

      // use clashing colors
      if (clashing){
        analogWrite(BLUELED, scale*blueCVal/255);
        analogWrite(GREENLED, scale*greenCVal/255);
        analogWrite(REDLED, scale*redCVal/255);
      }
      // use normal colors
      else{
        analogWrite(BLUELED, scale*blueVal/255);
        analogWrite(GREENLED, scale*greenVal/255);
        analogWrite(REDLED, scale*redVal/255);
      }
    }
}

// set conditions for fade on powerup
void LEDTurnOn()
{
 clashing = false;
 lengthTime = clipLengh();
 target = 255;
 last = 0;
 timetrigger = millis()+ lengthTime;
 updatelight();
}

// set conditions for fad off powerdown
void LEDfadeOff()
{
 lengthTime = clipLengh();
 target = 0;
 timetrigger = millis()+ lengthTime;
}

// turn off LEDs and reset
void LEDOFF()
{
  clashCount = 0;
  analogWrite(BLUELED, 0);
  analogWrite(GREENLED, 0);
  analogWrite(REDLED, 0);

  // reset to turn on color
  blueVal = 0;
  greenVal = 254;
  redVal = 0;
}

// start clash visuals
void detectedClashLED()
{
  clashCount ++;
  timetrigger = millis();
  clashStop = millis() + CLASHDUR;
  clashing = true;
  last = 254;

  // these are just visual demo of battle changing traits
  blueVal = 0;//100 - min(100, 10*clashCount);
  greenVal = 255-min(255, 255*clashCount/10);
  redVal = min(255*clashCount/10, 255);
}

