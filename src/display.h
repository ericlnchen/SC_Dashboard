#include <Arduino.h>
#include <U8g2lib.h>

enum diagnostics {oT,oP,cT,b};

class Display{
public:
    Display();
    virtual void initializeDisplay();
    virtual void drawBackground();
    virtual void drawBoxGauge(const unsigned int current, const unsigned int max, const unsigned int cutoff, const unsigned int redLine);
    virtual void functioning_battery(const double value, const bool isFunc);
    void functioning(const unsigned char value, const char diagnostic, const bool isFunc);
    void drawMph(const unsigned char mph);
    void drawGear(const char gear);
    void drawLapTime(int *lapTime);
protected:
    int screenx = 240;
    int screeny = 128;
    int fontx = 12;
    int fonty = 15;
    void clearBox(int x0, int y0, int w, int h);
    void drawDarkBox(int x0, int y0, int w, int h);
private:
    U8G2_T6963_240X128_F_8080 u8g2;
};