
long int buttonPressed;    // The mills() of the start of the last press of the button
                             // If this equals zero then the button isn't pressed
long int sinceButton;      //  The millis of the last release

int numberpresses;        //The number of times the button has been pressed

#define resetpresses 2000  //after 5 secounds reset presses
#define maxPressbutton 500

//Cheacks the momentary swich and updates variables
void updateSwich(){
  boolean buttonState = digitalRead(momentarypin);
  // if a long time since last press then update num of presses
  if (millis()-sinceButton > resetpresses){      
      numberpresses=0;
  }
  
  //When the button is first pressed
  if (buttonState&& buttonPressed==0){
      buttonPressed=millis();
  }
  // when the button is released
  else if (buttonState== false  && buttonPressed!=0) {
    if (millis()-buttonPressed < maxPressbutton){
      numberpresses++;
    }
    sinceButton = millis();
    buttonPressed = 0;
  }
}

void resetButton(){
  buttonPressed=millis();
}





//returns the time since the last button press
long int timeSinceLastPress(){
  return millis()-sinceButton;
}

// returns how long the button has ben pressed currently
int timePressed(){
 if (buttonPressed == 0){
   return 0;
 }
 else {
   return millis()-buttonPressed;
 }
}


int numPressed(){
  return numberpresses;
}
