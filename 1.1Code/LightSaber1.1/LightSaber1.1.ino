#include <TimerThree.h>
#include "Settings.h"
#include <SPI.h>
#include <SD.h>

const int CLASHTHRESH = 110;


enum State {standby, hum, swing, clash };

enum State currentState;
void setup() {
  Serial.begin(250000);
  Serial.println("On");
  pinMode(AUXA, OUTPUT);
  pinMode(GREENLED, OUTPUT);
  pinMode(BLUELED, OUTPUT);
  pinMode(REDLED, OUTPUT);
  pinMode(AMPSHUTDOWN, OUTPUT);
  digitalWrite(AUXA, LOW);
  setupAudio();
  Serial.println("conected to SD card");

  loadDCOffset();
  Timer3.initialize(SAMPLEPER);
  Timer3.attachInterrupt(gyroSample);
  currentState = standby;
  pinMode(MAINBUT, INPUT_PULLUP);


  Serial.println("setup done!");
  digitalWrite(REDLED, 1);
  digitalWrite(GREENLED, 1);
  digitalWrite(BLUELED, 1);

  clashSound();
  //setDCOffset();

  delay(1000);
  clashSound();
  digitalWrite(BLUELED, 0);
  digitalWrite(REDLED, 0);
  digitalWrite(GREENLED, 0);

}

void loop()
{
  static long int startPress = 0;
  static bool waiting = false;
  static elapsedMillis sampleSensors;



  //if (sampleSensors >= SAMPLEPER && currentState != standby) {
  //  sampleSensors = sampleSensors - SAMPLEPER;
  //  long int stuff = millis();
  //   gyroSample();
  //stuff = millis()-stuff;
  //Serial.print("time for sample:");
  //Serial.println(currentOmega());

  // }

  // start debouncing, only after trigbounce period past end of last
  if (!waiting && mainButtonPressed() && (millis() - startPress > TRIGBOUNCE)) {
    waiting = true;
    startPress = millis();
  }
  if (waiting && (millis() - startPress > TRIGBOUNCE) ) {
    waiting = false;
    startPress = millis();
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
      delay(1000);
      lowpower();
    }
    else if (currentState == standby && mainButtonPressed()) {
      Serial.println("poweron");
      randomSeed(millis());
      randomColor();
      powerUP();
      currentState = hum;
      playFile("poweron.WAV");
      LEDTurnOn();
      digitalWrite(22, HIGH);
      
    }
  }

  switch (currentState) {
    case standby:
      delay(10);
      break;
    case hum:
      updatelight();

      if (currentOmega() >= SWINGTHRESH) {
        swingSound();
        Serial.print("Swing:");
        Serial.println(currentOmega());

        currentState = swing;
      }
      else if (currentOmegaDot() >= CLASHTHRESH ) {
        randomColor();
        detectedClashLED();
        clashSound();
        Serial.println("Detected Clash");
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
      // detect stoped TODO
      //      if (currentOmega() <= SWINGEND) {
      //        playFile("hum.wav");
      //        currentState = hum;
      //      }
      // detect clash
      if (currentOmegaDot() >= CLASHTHRESH) {
        randomColor();
        detectedClashLED();
        clashSound();
        Serial.println("Detected Clash");
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

bool mainButtonPressed() {
  return (MAINBUTPUSH == digitalRead(MAINBUT));
}

void lowpower() {
  gyroOFF();
  soundDown();
}

void powerUP() {
  gyroON();
  soundON();
}
