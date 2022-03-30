#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>
#include <U8g2lib.h>
#include "TouchScreen.h"
enum diagnostics {oT,oP,cT,b};
void menuScreen();
void setupScreen();
void gpsScreen();
void naughtTo60Screen();
void initializeDisplay();
void clearBox(int x0, int y0, int w, int h);
void drawDarkBox(int x0, int y0, int w, int h);
void functioning(const unsigned char value, const char diagnostic, const bool isFunc);
void functioning_battery(const double value, const bool isFunc);
void drawMph(const unsigned char mph);
void drawBoxGauge(const unsigned int current, const unsigned int max, const unsigned int cutoff, const unsigned int redLine);
void drawGear(const char gear);
void drawLapTime(int *lapTime);
void drawBackground();
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A8  // must be an analog pin, use "An" notation!
#define YM A1  // can be a digital pin
#define XP A2   // can be a digital pin
const int backlightPin = 23;
#define MINPRESSURE 10
#define MAXPRESSURE 1000
#endif