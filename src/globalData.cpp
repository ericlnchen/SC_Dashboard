#include "globalData.h"

int current_throttle = 0;
int current_coolantTemp = 0;
int current_oilTemp = 0;
int current_oilPressure = 0;
float current_battery = 0;

int last_throttle = 0;
int last_coolantTemp = 0;
int last_oilTemp = 0;
int last_oilPressure = 0;
float last_battery = 0;
bool loggingActive = false;
bool loggingSuccessful = false;
loggingState loggingStatus = sdError;

unsigned int gpsUpdateTime = 0;
unsigned int serialExtractTime = 0;
unsigned int ioReadTime = 0;
unsigned int dataLogTime = 0;
unsigned int displayUpdateTime = 0;
unsigned int mainLoopTime = 0;