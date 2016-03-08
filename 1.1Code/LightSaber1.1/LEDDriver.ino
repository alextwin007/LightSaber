
 int delaymin =100;
const int delaymax =300;
const int brightMin= 200;

int blueVal = 100;
int greenVal = 254;
int redVal = 0;

int blueCVal = 254;
int greenCVal= 254;
int redCVal = 254;

const int CLASHDUR = 200;
const int CLASHMIN = 150;

#include "Settings.h"

long int lengthTime;
int target;
int last;
long int timetrigger;
bool clashing = false;
long int clashStop;

void randomColor()
{
  blueVal = random(0,255);
  greenVal = random(0,255);
  redVal = random(0,255);

  blueCVal = random(0,255);
  greenCVal = random(0,255);
  redCVal = random(0,255);
  
}
void updatelight(){
  long int currenttime = millis();
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
    else {
      
      int scale = target - float(timetrigger-currenttime)/(lengthTime)*(target-last);
;
      if (clashing){
        analogWrite(BLUELED, scale*blueCVal/255);
        analogWrite(GREENLED, scale*greenCVal/255);
        analogWrite(REDLED, scale*redCVal/255);
      }
      else{
        analogWrite(BLUELED, scale*blueVal/255);
        analogWrite(GREENLED, scale*greenVal/255);
        analogWrite(REDLED, scale*redVal/255);
      }
    }
}

void LEDTurnOn()
{
 lengthTime = clipLengh();
 target = 200;
 last = 0;
 timetrigger = millis()+ lengthTime;
 updatelight();
}

void LEDfadeOff()
{
 lengthTime = clipLengh();
 target = 0;
 timetrigger = millis()+ lengthTime;

}

void LEDOFF()
{
  analogWrite(BLUELED, 0);
  analogWrite(GREENLED, 0);
  analogWrite(REDLED, 0);
}

void detectedClashLED()
{
  timetrigger = millis();
  clashStop = millis() + CLASHDUR;
  clashing = true;
  last = 254;
}

