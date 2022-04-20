#ifndef GPS_H
#define GPS_H
#include "SparkFun_u-blox_GNSS_Arduino_Library.h" //http://librarymanager/All#SparkFun_u-blox_GNSS
#include <Wire.h> //Needed for I2C to GPS
void setupGPS();
extern float latitude;
extern float longitude;
extern float gpsSpeed;
extern int xAccel;
extern int yAccel;
extern int zAccel;
extern int gpsUpdateEvent;
extern int gpsRealUpdateRate;
extern int bestGpsUpdateRate;
extern int executionCounter;
extern int executionRate;
extern bool isGpsCalibrated; // use these flags to avoid wasting time
extern bool usingAutoHNRPVT;
extern bool usingAutoHNRDyn;
extern bool usingAutoHNRAtt;
extern bool GPSconnected;
extern bool performanceState;
extern bool newUpdate;

#endif //GPS_H