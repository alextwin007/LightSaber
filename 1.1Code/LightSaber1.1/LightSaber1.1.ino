#include <TimerThree.h>
#include "Settings.h"

enum State{idle, hum, swing, clash };

enum State currentState;
void setup(){
  Serial.begin(57600);
  setupAudio();
  Serial.println("conected to SD card");
  Timer3.initialize(SAMPLEPER);
  Timer3.attachInterrupt(gyroSample);
  loadDCOffset();
  currentState = hum;
}

void loop()
{
  //Serial.println(currentOmega());
  switch (currentState) {
    case idle:
      lowpower();
      break;
    case hum:
      // restart hum
      if (! Playing()){
        playFile("idle.wav");
      }
      if (currentOmega() >= SWINGTHRESH){
        currentState = swing;
        playFile("swing1.wav");
      }
      break;
      
    case swing:
      // detect stoped
      if (currentOmega() <= SWINGEND){
        playFile("idle.wav");
        currentState = hum; 
      }
      // start new swing
      else if (! Playing()){
        playFile("swing1.wav");
      }
      break;
      
    default:
     Serial.println("unknown State"); 

  }
}

void lowpower(){
  gyroOFF();
}

void powerUP(){
  gyroON(); 
}
