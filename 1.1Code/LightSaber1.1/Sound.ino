// this file contains the code for reading and playing the audio sand
//    from the SD card

//here we globaly declare the vaiables for the library

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "Settings.h"



// This is the name of our Wave player object
AudioPlaySdWav           playWav1;       //xy=154,78

// We are using the DAC output
AudioOutputAnalog        dac1;           //xy=608,248

// Conecting the DAC output to playWav1
AudioConnection          patchCord1(playWav1, dac1);

void setupAudio()
{

  AudioMemory(8);


  // Set the SPI pins
  SPI.setMISO(MISOPIN);
  SPI.setMOSI(MOSIPIN);
  SPI.setSCK(SCKPIN);
  
  // Conect to the SD card
  if (!(SD.begin(CSPIN))) {
    // stop here, but print a message repetitively
    while (1) {
      //TODO change to play some tone? or other messages
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
}


void playFile(const char *filename)
{
  playWav1.play(filename);

  // A brief delay for the library read WAV info
  delay(5);
}

boolean Playing()
{
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

