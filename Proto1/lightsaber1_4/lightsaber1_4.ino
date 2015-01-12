// Code for a lightsabor sound, driving system
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Garan.h"



#define bufferSize 100
#define windowSize 10
#define swingthresh 510 
#define clashthresh 50 //50
#define swingbreakout 16
#define clashbounce 550 //Time required between clashes in millis
#define momentarypin 10

#define startupbuttontime 1000
#define powerdownbuttontime 1000
#define pushmin 200
#define pushdelaymin 500
#define pushdelaymax

// Definitions for states of FSM
#define StandBy 0
#define PowerUp 1
#define Calibrate 2
#define Idle 3
#define Swing 4
#define Clash 5
#define Lockup 6
#define PowerDown 7
#define SaberLock 8



// Declaring global variables
int state;                 //  Variable holds state for FSM
int bufferFront;           //  The current buffer offset


float omegaBuff[bufferSize];  //  buffer of omega values
float alphaBuff[bufferSize];  //  buffer of alpha values

#if !defined(__MSP430_CPU__)
SoftwareSerial garanSerial(8,9 ); // RX, TX
#else
SoftwareSerial garanSerial(P1_4, P1_5); // RX, TX
#endif

Garan player(garanSerial);

// Init the starting values
void setup(){
  Serial.begin(9600);
  debugginit();
  player.stop();
  player.setVolume(16);
  player.feedbackAtEnd();
  pinMode(momentarypin, INPUT);
  
  // Need to set up initial window in the buffers
  setUpBuffer();
  state = StandBy;
  Serial.println("Calibrating");
  ///reCalibrate();
  loadCalibration();

}



// main FSM loop
void loop(){
  if (state != StandBy){
    updateBuff();
  }
  updateSwich();
  //debugprint();
  delay(1);
  switch (state) {
    case StandBy:
      StandByState();
      break;
    case PowerUp:
      PowerUpState();
      break;
    case Clash:
      ClashState();
      break;
    case Swing:
      SwingState();
      break;
    case PowerDown:
      PowerDownState();
      break;
    default: 
      IdleState();
  }
  
}

// This section contains all the FSM states
void PowerUpState(){
    if (DonePlaying()){                     
      state = Idle;
      Serial.println("Idle");
      player.singleLoopName("Idle.wav");
    }
    
}


//When the lightsaber is on
void IdleState(){
 int buttonHeld =timePressed();
 if (buttonHeld > powerdownbuttontime){
    state = PowerDown;
 }
 else if (numPressed()==3){
   Serial.println("calibration");
   reCalibrate();
   loadCalibration();
 }
 else if (detectClash()){
    state = Clash;
    Serial.println("**********Clash***********");
    player.singlePlayName("impact1.wav");
  }
  else if (detectSwing() != 0){
    state = Swing;
    Serial.println("************Swing*************");
    player.singlePlayName("motion1.wav");
  }
}

void ClashState(){
  if (DonePlaying()){
    state = Idle;
    player.singleLoopName("idle.wav");
    Serial.println("Idle");
  }
}



void SwingState(){
  Serial.println("swing");
  int sum = 0;
  for (int i=0; i<20; i++){
    updateBuff();
    if (detectClash()){
      state = Clash;
      Serial.println("********Clash------------");
      player.singlePlayName("impact1.wav");
      break;
    }
    if (detectSwing()==0){
      sum++;
    }
  }
  if (sum > swingbreakout){
    state = Idle;
    Serial.println("***********Idle--------------");
    player.singleLoopName("idle.wav");
  }
}

void SaberLockState(){
  if (detectSwing()!=0){
    state = Idle;
    Serial.println("---------------Idle--------------");
    player.singleLoopName("idle.wav");
  }
  // add replay if nessisary
}

void StandByState(){
  int buttonHeld =timePressed();
  boolean hi =detectClash();                            //remove this
  if (buttonHeld > startupbuttontime){
    state = PowerUp;
    player.singlePlayName("poweron.wav");
    Serial.println("Power On");
  }
    
}

void PowerDownState(){
  Serial.println("Power Down");
  player.singlePlayName("poweroff.wav");
  if (DonePlaying()){                     
      state = StandBy;
    }  
}




////////////////////////////////////////////////////////////////////////////////////////////
// Functions shared by many states
////////////////////////////////////////////////////////////////////////////////////////////

// Gets the current Omega
// performs moving adverage
// find the numerical derivative
void updateBuff(){
  float currentOmega = getCurrentOmega();
  float sum = currentOmega;
  for (int i=0; i < windowSize-1; i++){
    if (bufferFront - i >= 0){
      sum += omegaBuff[bufferFront-i];
    }
    else{
      int newindex = bufferFront-i+bufferSize;
      sum += omegaBuff[newindex];
    }
  }
  currentOmega = abs(sum/windowSize);
  
  // if at the end of the buffer
  if (bufferFront == bufferSize-1){
    bufferFront = 0;
    alphaBuff[bufferFront]=currentOmega-omegaBuff[bufferSize-1];
  }
    
  // does not have to deal with wraparound
  else {
    bufferFront++;
    alphaBuff[bufferFront]=currentOmega-omegaBuff[bufferFront-1];
  }
  //Serial.println(alphaBuff[bufferFront]);
  omegaBuff[bufferFront]=currentOmega;  
}





int detectSwing(){
  float val=omegaBuff[bufferFront]-swingthresh;
  //Serial.print('\t');
  //Serial.println(val);
  //delay(1);
  if (val>0){
    return val*100;
  }
  else {
    return 0;
  }
}

boolean detectClash(){
  float val = abs(alphaBuff[bufferFront]) - clashthresh;
  //Serial.print(getCurrentOmega());
  //Serial.print("\t");
  //Serial.print(omegaBuff[bufferFront]);
  //Serial.print("\t");
  //Serial.println(alphaBuff[bufferFront]);
  return val > 0;
}






void setUpBuffer(){
  for ( bufferFront = 0; bufferFront < windowSize; bufferFront++){
      float currentOmega = getCurrentOmega();
      float sum = currentOmega;
      for (int j = 0; j < bufferFront; j++){
        sum += omegaBuff[j];
      }
      omegaBuff[bufferFront]=sum/(bufferFront+1);
      if (bufferFront == 0){
        alphaBuff[bufferFront]=0;
      }
      else {
        alphaBuff[bufferFront]=omegaBuff[bufferFront]-omegaBuff[bufferFront-1];
      }    
    }
}





//////////////////////////////////////////////////////////////////////////////////////
// Functions to help with debugging




void debugginit(){

while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
}

// Prints the inputs of the raw gyro to
void debugprint(){
  Serial.print(millis());
  Serial.print(", \t");
  Serial.print(omegaBuff[bufferFront]);
  Serial.print(", \t");
  Serial.println(alphaBuff[bufferFront]);
}
