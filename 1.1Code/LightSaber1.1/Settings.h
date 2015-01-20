// Here is all the settings that are designed to be easily changed by the user.

// Pins conected to the SD card reader, follows standard SPI naming conveintions

#define MOSIPIN 7
#define MISOPIN 8
#define SCKPIN 14
#define CSPIN 9


// Gyro input pins, the analog values
#define XPIN 1
#define YPIN 2

// Gyro PD pin, power down pin
#define PDPINGYRO 10


//The amount of millisecounds between gyro samples
#define SAMPLEPER 50

//The size of the moving adverage filture of the swingvalues
#define SWINGBUFFSIZE 20
// Swing sense, lower is more sensitive
#define SWINGTHRESH 500

// This point we end the swing
#define SWINGEND 200

#define CLASHTHRESH 12
