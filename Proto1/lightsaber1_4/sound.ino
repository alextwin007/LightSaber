#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Garan.h"




#define statuspin 0
boolean DonePlaying(){
    if (garanSerial.available()) {
      unsigned char recvByte = garanSerial.read();
      if (recvByte == 0x24) Serial.println();
       if ((recvByte==0xf0)||(recvByte==0xf1)){
          return true;
       }
     return false;
    }
}
