//#define xOffset 365    //Data from sensor calibration
//#define zOffset 345    // Data from sensor calibration
//#define mult 0.0032 // The volt for a unit returned by ADC
#define Xpin 3
#define Zpin 1

#define xStoreA 1
#define zStoreA 2
#define shareStore 3


#include <EEPROM.h>
int xOffset;
int zOffset;

float getCurrentOmega(){
  // sense is a user selected sensitivity
  float XOmega = (analogRead(Xpin)-xOffset);//*sense*mult;
  float ZOmega = (analogRead(Zpin)- zOffset);//*sense*mult;
  if (abs(XOmega) > 1000){  //what?
      XOmega = 0;
  }
  // remove this
  float sum = sqrt(sq(XOmega)+sq(ZOmega));
  //Serial.println(XOmega);
  return sum;
  
}

void loadCalibration(){
  int share = EEPROM.read(shareStore); 
  int xShare = share%4;
  int zShare = (share/4)%4;
  xOffset = xShare*256+EEPROM.read(xStoreA);
  zOffset = zShare*256+EEPROM.read(zStoreA);
  Serial.print("xOffset = ");
  Serial.println(xOffset);
  Serial.print("zOffset = ");
  Serial.println(zOffset);
  
;
}

void reCalibrate(){
  long int xSum = 0;
  long int zSum = 0;
  for (int i = 0; i<10000; i++){
    xSum += analogRead(Xpin);
    zSum += analogRead(Zpin);
    delay(1);
  }
  xOffset = xSum/10000;
  zOffset = zSum/10000;
  Serial.print("Xoffset = ");
  Serial.println(xOffset);
  Serial.print("Xsum = ");
  Serial.println(xSum);
  EEPROM.write(xStoreA,xOffset%256);
  EEPROM.write(zStoreA, zOffset%256);
  int xShare = xOffset/256;
  int zShare = (zOffset/256)*4;
  EEPROM.write(shareStore, xShare+zShare);
  
  
}
