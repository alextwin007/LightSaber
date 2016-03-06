#include <EEPROM.h>
#include "Settings.h"


// the Gyro offsets
int xOffset;
int yOffset;

// Memory addreses used by the Gyro offsets
#define SHARESTORE 1
#define XSTOREA 2
#define YSTOREA 3

// The buffer that is used for finding the swing
unsigned int omegaBuff[SWINGBUFFSIZE];
unsigned long int omegaBuffSum = 0;
unsigned int omegaDot = 0;

// Loads the DC offset from the eeprom
void loadDCOffset()
{
  int share = EEPROM.read(SHARESTORE); 
  int xShare = share%4;
  int yShare = (share/4)%4;
  xOffset = xShare*256+EEPROM.read(XSTOREA);
  yOffset = yShare*256+EEPROM.read(YSTOREA);
  Serial.print("xOffset = ");
  Serial.println(xOffset);
  Serial.print("yOffset = ");
  Serial.println(yOffset);
  // turn off Gyro
  pinMode(PDPINGYRO, OUTPUT);
  digitalWrite(PDPINGYRO, LOW);
}

// Re-sets the DC offset of the Gyros
void setDCOffset(){
  long int xSum = 0;
  long int ySum = 0;
  // take a lot of samples
  for (int i = 0; i<10000; i++){
    xSum += analogRead(XPIN);
    ySum += analogRead(YPIN);
    delay(1);
  }
  xOffset = xSum/10000;
  yOffset = ySum/10000;
  Serial.print("Xoffset = ");
  Serial.println(xOffset);
  Serial.print("Xsum = ");
  Serial.println(xSum);
  
  // spit up the offsets and store them for later
  EEPROM.write(XSTOREA,xOffset%256);
  EEPROM.write(YSTOREA, yOffset%256);
  int xShare = xOffset/256;
  int yShare = (yOffset/256)*4;
  EEPROM.write(SHARESTORE, xShare+yShare);  
}

void gyroSample()
{

 static unsigned short int index = 0;

 
 signed int oldValue = omegaBuff[index];
 signed int newValue = gyroMag();
 //unsigned int oldOmega = currentOmega();
 omegaBuffSum = omegaBuffSum + newValue - oldValue;
 omegaBuff[index] = newValue;
 index = (index + 1) % SWINGBUFFSIZE;

 omegaDot = abs(oldValue-newValue);
}

unsigned int gyroMag()
{
  int x = analogRead(XPIN) - xOffset;
  int y = analogRead(YPIN) - yOffset;
  return (unsigned int) sqrt(sq(x)+sq(y));
}

unsigned int currentOmega()
{
 return omegaBuffSum/SWINGBUFFSIZE;
}

unsigned int currentOmegaDot()
{
  return omegaDot*10/SWINGBUFFSIZE;
}
void gyroON()
{
   digitalWrite(PDPINGYRO, LOW);
}

void gyroOFF()
{
   digitalWrite(PDPINGYRO, HIGH);
}
