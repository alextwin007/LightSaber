// Created by Alex Alves
// Modified March 14, 2016

// this is under Open Source License

// This is free software. You can redistribute it and/or modify it under
//    the terms of Creative Commons Attribution 3.0 United States License.
//    To view a copy of this license, visit
//    http://creativecommons.org/licenses/by/3.0/us/


// The intention of this is to provides something so that everyone, from a DIYer, to skilled sabersmiths can customize 
//    features, software, and hardware to the needs of customers.  The intention is that such modification will be
//    shared with the lightsaber community.  Note that mass distribution of this code for purely individual monetary
//    gain without sharing would be frowned upon by the creators of this code.  
//
//Not associated with LUCASFILM LTD.

#include <TimerThree.h>
#include "Settings.h"
#include <SPI.h>
#include <SD.h>

  int CLASHTHRESH = 120;

// states of FSM
enum State {standby, hum, swing, clash };

enum State currentState;
void setup() {

  // Turn on debugging serial usb and turn on LED's
  Serial.begin(250000);
  Serial.println("On");

  // set up the output pins
  pinMode(AUXA, OUTPUT);
  pinMode(GREENLED, OUTPUT);
  pinMode(BLUELED, OUTPUT);
  pinMode(REDLED, OUTPUT);
  pinMode(AMPSHUTDOWN, OUTPUT);
  digitalWrite(AUXA, LOW);
  setupAudio();
  Serial.println("conected to SD card");

  // load DC Offset from EEPROM
  loadDCOffset();

  // Timer3 used for sampling of data, ensures equal timing
  Timer3.initialize(SAMPLEPER);
  Timer3.attachInterrupt(gyroSample);


  //Timer3.stop();

  // standby is waiting
  currentState = standby;

  // turn on internal pullup for momentary swich

  pinMode(MAINBUT, INPUT_PULLUP);


  Serial.println("setup done!");

  // turn on all colors, test ability to show all dies
  digitalWrite(REDLED, 1);
  digitalWrite(GREENLED, 1);
  digitalWrite(BLUELED, 1);

  // test for sound
  clashSound();


  //setDCOffset();

  delay(1000);

  // turn off blade LED
  digitalWrite(BLUELED, 0);
  digitalWrite(REDLED, 0);
  digitalWrite(GREENLED, 0);

}

void loop()
{
  static long int startPress = 0;
  static bool waiting = false;
  static elapsedMillis sampleSensors;

  //
  //
  //  if (sampleSensors >= SAMPLEPER && currentState != standby) {
  //    Serial.println(sampleSensors);
  //    sampleSensors = sampleSensors - SAMPLEPER;
  //    //long int stuff = millis();
  //     gyroSample();
  //  //stuff = millis()-stuff;
  //  //Serial.print("time for sample:");
  //  //Serial.println(currentOmega());
  //
  //   }

  // start debouncing, only after trigbounce period past end of last
  if (!waiting && mainButtonPressed() && (millis() - startPress > TRIGBOUNCE)) {
    waiting = true;
    startPress = millis();
  }

  // detected press of length TRIGBOUNCE
  if (waiting && (millis() - startPress > TRIGBOUNCE) ) {
    waiting = false;
    startPress = millis();
    // Saber is on, turning off
    if (mainButtonPressed() && currentState != standby) {
      currentState = standby;
      playFile("poweroff.WAV");
      LEDfadeOff();
      Serial.println("poweroff");
      while (Playing()) {
        updatelight();
        delay(10);
      }
      Serial.println("in standby");
      digitalWrite(22, LOW);
      LEDOFF();
      //Timer3.stop();
      noInterrupts();
      delay(1000);
      lowpower();
    }
    // Saber is off, start startup sequence
    else if (currentState == standby && mainButtonPressed()) {
      powerUP();

      // Set random seed
      randomSeed(millis());

      //randomColor();    // uncoment this to get random color profile at power on
      Serial.println("poweron");

      // start in normal on state
      currentState = hum;
      playFile("poweron.wav");

      // initialize fade on
      LEDTurnOn();

      // turn on indicator LED
      digitalWrite(22, HIGH);

      // allow samplining to begin
      interrupts();

      // wait to prevent triggering clash at startup
      delay(40);

    }
  }

  // main FSM
  switch (currentState) {
    case standby:   // Power off
      delay(10);
      break;
    case hum:       // Power on, not clashing or swinging
      updatelight();

      // detected new swing
      if (currentOmega() >= SWINGTHRESH) {
        swingSound();
        Serial.print("Swing:");
        Serial.println(currentOmega());
        currentState = swing;
      }

      // detected new clash
      else if (currentOmegaDot() >= CLASHTHRESH ) {
        detectedClashLED();
        clashSound();
        Serial.print("Detected Clash1: ");
        Serial.println(currentOmegaDot());
        currentState = clash;
      }
      // restart hum
      else if (! Playing()) {
        playFile("hum.wav");
        Serial.println("Replay hum");

      }
      else {
        delay(10);
      }
      break;

    case swing:
      updatelight();
      // detect clash
      if (currentOmegaDot() >= CLASHTHRESH) {
        //randomColor();
        detectedClashLED();
        clashSound();
        Serial.print("Detected Clash2: ");
        Serial.println(currentOmegaDot());
        currentState = clash;
      }
      // start new swing
      else if (! Playing()) {
        if (currentOmega() >= SWINGTHRESH) {
          swingSound();
          Serial.println("Swing again");
        }
        else {
          currentState = hum;
          playFile("hum.wav");
        }

      }

      break;

    case clash:
      updatelight();
      // return to swing
      if (! Playing()) {
        playFile("swing1.wav");
        Serial.println("exit clash");
        currentState = swing;
      }
      else {
        delay(1);
      }
      break;

    default:
      Serial.println("unknown State");

  }
}

// is main button currently pressed down
bool mainButtonPressed() {
  return (MAINBUTPUSH == digitalRead(MAINBUT));
}

// turn off as many things as possible
void lowpower() {
  gyroOFF();
  soundDown();
}

// wake up everything that was turned off
void powerUP() {
  gyroON();
  soundON();
}
