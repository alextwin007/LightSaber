// Here is all the settings that are designed to be easily changed by the user.

// Pins conected to the SD card reader, follows standard SPI naming conveintions

#define MOSIPIN 7
#define MISOPIN 8
#define SCKPIN 14
#define CSPIN 9

#define MAINBUT 23
#define MAINBUTPUSH LOW

#define SWINGNUM 9
#define CLASHNUM 9

// Gyro input pins, the analog values
#define XPIN 1
#define YPIN 2

// Gyro PD pin, power down pin
#define PDPINGYRO 10

//The amount of millisecounds between gyro samples
#define SAMPLEPER 50

//The size of the moving adverage filture of the swingvalues
#define SWINGBUFFSIZE 6
// Swing sense, lower is more sensitive
#define SWINGTHRESH 700

// This point we end the swing, not being used currently
#define SWINGEND 0


#define AUXA 22

#define TRIGBOUNCE 500

#define ONTIME 3000

#define BLUELED 21
#define GREENLED 20
#define REDLED 3

// amp shutdown pin
#define AMPSHUTDOWN 12
