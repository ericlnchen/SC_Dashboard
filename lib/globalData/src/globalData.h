#ifndef GLOBALDATA_H
#define GLOBALDATA_H
#include <Arduino.h>
#include <U8g2lib.h>
#include <string>
#include "TouchScreen.h"

class Draw{
public:
    U8G2_T6963_240X128_F_8080 u8g2;
    Draw();
    void clearBox(int x0, int y0, int w, int h);
};

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

extern TSPoint p;
extern Draw painter;

#endif