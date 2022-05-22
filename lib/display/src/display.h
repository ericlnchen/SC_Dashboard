#ifndef DISPLAY_H
#define DISPLAY_H
#include <globalData.h>
#include <touch.hpp>


class Display{
public:
    Display();
    virtual void initializeDisplay();
    virtual void drawBackground();
    virtual void drawBoxGauge(const unsigned int current, const unsigned int max, const unsigned int cutoff, const unsigned int redLine);
    void display_coolantTemp(const unsigned char value, const bool isFunc);
    void display_oilTemp(const unsigned char value, const bool isFunc);
    void display_oilPressure(const unsigned char value, const bool isFunc);
    void display_batteryVoltage(const float value, const bool isFunc);
    void drawMph(const unsigned char mph);
    void drawGear(const char gear);
    void drawLapTime(int *lapTime);
    void sendAll();

protected:
    button mb = button("Menu", 190, 110);
    int screenx = 240;
    int screeny = 128;
    int fontx = 12;
    int fonty = 15;
    void clearBox(int x0, int y0, int w, int h);
    void drawDarkBox(int x0, int y0, int w, int h);
    unsigned long lastDisplayUpdate = 0;
};

#endif