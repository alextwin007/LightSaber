// this file contains the code for reading and playing the audio sand
//    from the SD card

//here we globaly declare the vaiables for the library

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "Settings.h"

#define SOUNDGAIN 0.95  // gain at mixer, 1 is unity, can be in range 0<x<4

// char array holding the names of sound files
const char* swingName[] = {"swing1.wav", "swing2.wav", "swing3.wav", "swing4.wav", "swing5.wav", "swing6.wav", "swing7.wav", "swing8.wav", "swing9.wav"};
const char* clashName[] = {"clash1.wav", "clash2.wav", "clash3.wav", "clash4.wav", "clash5.wav", "clash6.wav", "clash7.wav",  "clash9.wav"}; // "clash8.wav",

// This is the name of our Wave player object
AudioPlaySdWav           playSdWav1;       //xy=154,78

// We are using the DAC output
AudioOutputAnalog        dac1;         



AudioMixer4              mixer1;
AudioConnection          patchCord1(playSdWav1, 0, mixer1, 0);
AudioConnection          patchCord2(mixer1, dac1);



// Audio setup with SD and amp off
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
    for (int i = 0; i < 10; i++) {
      digitalWrite(AUXA, HIGH);
      delay(250);
      digitalWrite(AUXA, LOW);
      delay(250);
    }
  }
  // set sound gain
  mixer1.gain(0, SOUNDGAIN);
}


// play a specific filename
void playFile(const char *filename)
{
  checkSD();
  playSdWav1.play(filename);
  //interrupts();
  // A brief delay for the library read WAV info
  delay(5);
}

// returns if device is curently playing sound
boolean Playing()
{

  if (checkSD()) {
    return (playSdWav1.isPlaying());
  }
  else {
    return false;
  }
}

// Check that dummy file is on SD
//    allows better debugging
//    if SD not detected AUXA will blink
boolean checkSD()
{
  static bool LEDState = 0;
  AudioNoInterrupts();
  if (!SD.exists("example.txt")) {
    Serial.println("Cannot find file");
    Serial.flush();
    delay(250);
    digitalWrite(AUXA, LEDState);
    LEDState = !LEDState;
    AudioInterrupts();
    return false;
  }
  AudioInterrupts();
  return true;
}

// length of currently playing sound
int clipLengh()
{
  return (playSdWav1.lengthMillis());
}

// turn off audio amp
void soundDown()
{
  digitalWrite(AMPSHUTDOWN, LOW);
}

// turn on audio amp
void soundON()
{
  digitalWrite(AMPSHUTDOWN, HIGH);
}

// plays random swing sound
void swingSound()
{
  int chosenOne = random(0, SWINGNUM - 1);
  playFile(swingName[chosenOne]);
}

// plays random clash sound
void clashSound()
{
  int chosenOne = random(0, CLASHNUM - 1);
  playFile(clashName[chosenOne]);
}


