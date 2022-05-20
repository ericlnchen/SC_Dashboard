#ifndef GLOBALDATA_H
#define GLOBALDATA_H
#include <Arduino.h>

// IMPORTANT control variables
extern int current_throttle;
extern int current_coolantTemp;
extern int current_oilTemp;
extern int current_oilPressure;
extern float current_battery;

extern int last_throttle;
extern int last_coolantTemp;
extern int last_oilTemp;
extern int last_oilPressure;
extern float last_battery;

extern int logDifference;
extern bool newLog;
extern bool loggingActive;
extern bool loggingSuccessful;
enum loggingState {logRunning,sdError,dirError,fileError,loggingOff};
extern loggingState loggingStatus;


#endif