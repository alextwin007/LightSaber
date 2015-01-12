
const int pin =6;
const int delaymin =300; //here
const int delaymax =500;
const int brightMin= 235;



void updatelight(){
  static long int trigger;
  static int lengthTime;
  static int target;
  static int last;

  long int currenttime = millis();
    if (currenttime >= trigger) {
      int lengthTime = random(delaymin,delaymax);
      trigger = currenttime+lengthTime;
      last =target;
      int brightness = random(brightMin, 255)*0.66;
      target=brightness;
    }
    else {
      analogWrite(pin, int((trigger-currenttime)/lengthTime*(target-last) +last));
    }

}

#define numDies 2
const int Apin[numDies]={6, 5};
const int Amin = 92; //Percent of the min

//void Aupdatelight(){
//  static long int trigger;
//  static int lengthTime;
//  static int target;
//  static int last;
//
//  long int currenttime = millis();
//    if (currenttime >= trigger) {
//      int lengthTime = random(delaymin,delaymax);
//      trigger = currenttime+lengthTime;
//      last =target;
//      int brightness = random(92, 100)*0.66; //remove the 0.66
//      target=brightness;
//    }
//    else {
//      for (int i=0; i<numDies; i++){
//        analogWrite(pin[i], int((trigger-currenttime)/lengthTime*(target-last)+last));
//      }
//      
//      
//      //analogWrite(pin, int((trigger-currenttime)/lengthTime*(target-last) +last));
//    }
//
//}

