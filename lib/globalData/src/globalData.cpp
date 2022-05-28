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

// TSPoint p;
Draw painter = Draw();

int longDifference = 500;
bool loggingActive = false;
bool loggingSuccessful = false;
bool newLog = true;
loggingState loggingStatus = sdError;

unsigned int gpsUpdateTime = 0;
unsigned int serialExtractTime = 0;
unsigned int ioReadTime = 0;
unsigned int dataLogTime = 0;
unsigned int displayUpdateTime = 0;
unsigned int mainLoopTime = 0;

Draw::Draw() :
u8g2(U8G2_R2, 2, 14, 7, 8, 6, 20, 21, 5, /*enable/wr=*/ 27 , /*cs/ce=*/ 26, /*dc=*/ 25, /*reset=*/24) // Connect RD (orange) with +5V, FS0 and FS1 with GND
{
}
void Draw::clearBox(int x0, int y0, int w, int h){
    u8g2.setDrawColor(0);
    u8g2.drawBox(x0,y0,w,h);
    u8g2.setDrawColor(1);
}