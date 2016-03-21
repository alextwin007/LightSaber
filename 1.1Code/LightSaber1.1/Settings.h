// Here is all the settings that are designed to be easily changed by the user.

// Pins conected to the SD card reader, follows standard SPI naming conveintions
#define MOSIPIN 7
#define MISOPIN 8
#define SCKPIN 14
#define CSPIN 9

// Main LED pins, must be PWM.  
//    *Note cannot use pins 25, 32 with Timer3 for interupts
//    *Note cannot use pins  3,  4 with Timer1 for interupts
#define BLUELED 21
#define GREENLED 20
#define REDLED 3

// Pins for main buttons
#define MAINBUT 23      // swich conected pin
#define MAINBUTPUSH LOW // value low or high indicating press
#define AUXA 22         // illuminated swich 1

// Gyro input pins, the analog values
#define XPIN 1
#define YPIN 2
#define PDPINGYRO 10  // Gyro PD pin, power down pin



// number of swing and clash sounds respectivly
#define SWINGNUM 9
#define CLASHNUM 9




// Swing detection settings

#define SAMPLEPER 200     //The amount of micro secounds between gyro samples, MIN 50
#define SWINGBUFFSIZE 6   //The size of the moving adverage filter of the swingvalues
#define SWINGTHRESH 700   // Swing sense, lower is more sensitive
// #define SWINGEND 0     // This point we end the swing, not being used currently



// Time delay for triger press in millis
// trigger must be pressed this long to turn off or on
#define TRIGBOUNCE 500

//prevents turn off until amount of time in millis after device is turned on
#define ONTIME 3000



// amp shutdown pin
#define AMPSHUTDOWN 12
