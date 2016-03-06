#include <TimerThree.h>
#include "Settings.h"


char* swingName[] = {"swing1.wav", "swing2.wav", "swing3.wav", "swing4.wav", "swing5.wav", "swing6.wav", "swing7.wav", "swing8.wav", "swing9.wav"};
enum State{standby, hum, swing, clash };

enum State currentState;
void setup(){
  Serial.begin(57600);
  Serial.println("On");
  Serial.flush();
  pinMode(22, OUTPUT);
  pinMode(GREENLED, OUTPUT);
  pinMode(BLUELED,OUTPUT);
  pinMode(REDLED, OUTPUT);
  pinMode(AMPSHUTDOWN, OUTPUT);
  digitalWrite(22, LOW);
  setupAudio();
  Serial.println("conected to SD card");
  Timer3.initialize(SAMPLEPER);
  Timer3.attachInterrupt(gyroSample);
  loadDCOffset();
  currentState = standby;
  pinMode(MAINBUT,INPUT_PULLUP);
  
  
  Serial.println("setup done!");
  digitalWrite(REDLED,1);
  digitalWrite(GREENLED, 1);
  digitalWrite(BLUELED, 1);
  //setDCOffset();
  delay(1000);
  digitalWrite(BLUELED, 0);
  digitalWrite(REDLED,0);
  digitalWrite(GREENLED, 0);
  
}

void loop()
{
  static long int startPress=0;
  static bool waiting = false;
  // start debouncing, only after trigbounce period past end of last
  if (!waiting && mainButtonPressed()&& (millis()-startPress > TRIGBOUNCE)){
    waiting = true;
    startPress = millis();
  }
  if (waiting && (millis()-startPress > TRIGBOUNCE) ){  
    waiting = false;
    startPress = millis();
    if(mainButtonPressed() && currentState != standby){
        currentState = standby;
        playFile("poweroff.WAV");
        LEDfadeOff();
        digitalWrite(22, LOW);
        Serial.println("poweroff");
        while (Playing()){
          updatelight();
          delay(1);
        }
        LEDOFF();
        lowpower();
        delay(1000);
      }
      else if (currentState == standby && mainButtonPressed()){
        Serial.println("poweron");
        powerUP();
        currentState = hum;
        playFile("poweron.WAV");
        LEDTurnOn();
        digitalWrite(22, HIGH);  
        randomSeed(millis());
      }
  }

  switch (currentState) {
    case standby:
        delay(10);
      break;

    
    case clash:
      updatelight();
      if (! Playing()){
        Serial.println("exit clash");
        playFile("hum.WAV");
        currentState = hum;
      }
      break;

        
    case hum:
      // restart hum
      updatelight();
      if (! Playing()){
        Serial.println("Replay hum");
        playFile("hum.WAV");
      }
      if (currentOmega() >= SWINGTHRESH){
        currentState = swing;
        Serial.print("Swing:");
        Serial.println(currentOmega());
        int chosenOne = random(0, SWINGNUM-1);
        playFile(swingName[chosenOne]);
      }
      break;
      
    case swing:
      updatelight();
      // detect stoped TODO
      if (currentOmega() <= SWINGEND){
        playFile("hum.wav");
        currentState = hum; 
      }
      // detect clash
      else if(currentOmegaDot() >= CLASHTHRESH){
        detectedClashLED();
        Serial.println("Detected Clash");
        Serial.println(currentOmegaDot());
        playFile("clash1.wav");
        currentState = clash;
      }
      // start new swing
      else if (! Playing()){
        if (currentOmega() >= SWINGTHRESH){
          playFile("swing1.wav");
        }
        else{
          currentState = hum;
          playFile("hum.wav");
        }
        
      }
      
      break;

      
    default:
     Serial.println("unknown State"); 

  }
}

bool mainButtonPressed(){
  return (MAINBUTPUSH == digitalRead(MAINBUT));
}

void lowpower(){
  gyroOFF();
  soundDown();
}

void powerUP(){
  gyroON(); 
  soundON();
}
