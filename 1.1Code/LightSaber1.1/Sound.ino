// this file contains the code for reading and playing the audio sand
//    from the SD card

//here we globaly declare the vaiables for the library

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "Settings.h"

const char* swingName[] = {"swing1.wav", "swing2.wav", "swing3.wav", "swing4.wav", "swing5.wav", "swing6.wav", "swing7.wav", "swing8.wav", "swing9.wav"};
const char* clashName[] = {"clash1.wav", "clash2.wav", "clash3.wav", "clash4.wav", "clash5.wav", "clash6.wav", "clash7.wav",  "clash9.wav"}; // "clash8.wav",

// This is the name of our Wave player object
AudioPlaySdWav           playWav1;       //xy=154,78

// We are using the DAC output
AudioOutputAnalog        dac1;           //xy=608,248

// Conecting the DAC output to playWav1
AudioConnection          patchCord1(playWav1, dac1);

void setupAudio()
{

  AudioMemory(16);


  // Set the SPI pins
  SPI.setMISO(MISOPIN);
  SPI.setMOSI(MOSIPIN);
  SPI.setSCK(SCKPIN);
  
  // Conect to the SD card
  while (!(SD.begin(CSPIN))) {
    // stop here, but print a message repetitively
    //while (1) {
      //TODO change to play some tone? or other messages
      Serial.println("Unable to access the SD card");
      for (int i = 0; i<10; i++){
      digitalWrite(AUXA, HIGH);
      delay(250);
      digitalWrite(AUXA, LOW);
      delay(250);
      }
  }
}


void playFile(const char *filename)
{
  //noInterrupts();
  playWav1.play(filename);
  //interrupts();
  // A brief delay for the library read WAV info
  delay(5);
}

boolean Playing()
{
 static bool LEDState = 0;
 AudioNoInterrupts(); 
 if (!SD.exists("example.txt")){
  Serial.println("Cannot find file");
  delay(250);
  digitalWrite(AUXA, LEDState);
  LEDState = !LEDState;
 }
 AudioInterrupts(); 
 
 return (playWav1.isPlaying()); 
}

int clipLengh()
{
  return (playWav1.lengthMillis());
}

void soundDown()
{
  digitalWrite(AMPSHUTDOWN, LOW);
}

void soundON()
{
  digitalWrite(AMPSHUTDOWN, HIGH);
}

void swingSound()
{
  int chosenOne = random(0, SWINGNUM - 1);
  playFile(swingName[chosenOne]);
}

void clashSound()
{
  int chosenOne = random(0, CLASHNUM - 1);
  playFile(clashName[chosenOne]);
}


