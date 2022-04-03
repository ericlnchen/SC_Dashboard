#include "display.h"
#include <string>
#include <math.h>
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A8  // must be an analog pin, use "An" notation!
#define YM A1  // can be a digital pin
#define XP A2   // can be a digital pin
#define MINPRESSURE 10
#define MAXPRESSURE 1000

Display::Display() :
    u8g2(U8G2_R2, 2, 14, 7, 8, 6, 20, 21, 5, /*enable/wr=*/ 27 , /*cs/ce=*/ 26, /*dc=*/ 25, /*reset=*/24) // Connect RD (orange) with +5V, FS0 and FS1 with GND
{
}

void Display::initializeDisplay()
{
    pinMode(23,OUTPUT);
    analogWrite(23,100);
    u8g2.begin();
    u8g2.setContrast(255);
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvR12_tf);
    u8g2.drawStr(32,18,"Driver Booting");
    u8g2.sendBuffer();
    delay(3000);
    drawBackground();
    drawBoxGauge(0, 12000, 6000,10000);
    drawGear('N');
    u8g2.sendBuffer();
}

void Display::clearBox(int x0, int y0, int w, int h) {

    u8g2.setDrawColor(0);
    u8g2.drawBox(x0,y0,w,h);
    u8g2.setDrawColor(1);
}

void Display::drawDarkBox(int x0, int y0, int w, int h) {
    u8g2.setDrawColor(1);
    u8g2.drawBox(x0,y0,w,h);
    u8g2.setDrawColor(0);
}

void Display::drawBackground() {
    int fontx = 12;
    int x0 = screenx/2;
    int y0 = screeny/2;

    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.drawStr(x0 - 5,y0 + 43, "mph");

    u8g2.drawFrame(x0 - 16,y0 + 7,40,40); // mph
    u8g2.drawFrame(x0 - 16,y0 - 43,40,51); // gear
    u8g2.drawFrame(0,0,screenx,22); // gauge
    u8g2.drawLine(0,screeny - 74,104,screeny - 74); // left first horizontal line
    u8g2.drawLine(0,screeny - 42,104,screeny - 42); // left second horizontal line
    u8g2.drawLine(0,110,104,110); // left third horizontal line
    u8g2.drawLine(140,110,240,110); // right bottom hozizontal line
    u8g2.drawVLine(53,20,66); // left vertical line
    u8g2.setFont(u8g2_font_logisoso16_tr);
    u8g2.drawStr(147, 50, "RPM");

    u8g2.setFont(u8g2_font_VCR_OSD_mf);
    u8g2.drawStr(2 + fontx*4-5,109,":");

}

void Display::functioning_battery(const double value,const bool isFunc){

    if(isFunc) clearBox(54,55,50,31);
    else drawDarkBox(54,55,50,31);
    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.drawStr(2+53,screeny - 63,String("Battery").c_str());
    u8g2.setFont(u8g2_font_VCR_OSD_mf);
    u8g2.setFontMode(1);
    int x0 = screenx/2;
    int y0 = screeny/2;
    if(value < 10){
        u8g2.drawStr(x0-64 + fontx,y0+21,String(int(value)).c_str());
    }
    else{
        u8g2.drawStr(x0-64,y0+21,String(int(value)).c_str());
    }
    int firstDecimal = int(value*10.0) % 10;
    u8g2.drawStr(x0 - 35, y0+21,String(firstDecimal).c_str());
    u8g2.drawStr(x0-64+fontx*2-fontx/4,y0+21,".");
    u8g2.setFontMode(0);
}

void Display::functioning(const unsigned char value, const char diagnostic, const bool isFunc){

    if(diagnostic == cT){
        if(isFunc) clearBox(0,22,53,32);
        else drawDarkBox(0,22,53,32);
        u8g2.setFont(u8g2_font_helvR08_tf);
        u8g2.drawStr(2,screeny - 93,String("Coolant").c_str());
        u8g2.setFont(u8g2_font_VCR_OSD_mf);
        u8g2.drawStr(fontx*3,screeny - 75,"C");
        int x0 = (value>=100) ? 0 : fontx;
        int y0 = screeny-fonty;
        u8g2.drawStr(x0,y0 - 60, String(value).c_str());
    }
    if(diagnostic == oT){
        if(isFunc) clearBox(0,55,53,31);
        else drawDarkBox(0,55,53,31);
        u8g2.setFont(u8g2_font_helvR08_tf);
        u8g2.drawStr(2,screeny - 63,String("Oil").c_str());
        u8g2.setFont(u8g2_font_VCR_OSD_mf);
        u8g2.drawStr(fontx*3,screeny - 43,"C");
        int x0 = (value>=100) ? 0 : fontx;
        int y0 = screeny;
        u8g2.drawStr(x0,y0-43,String(value).c_str());
    }
    if(diagnostic == oP){
        if(isFunc) clearBox(54,22,50,32);
        else drawDarkBox(54,22,50,32);
        u8g2.setFont(u8g2_font_helvR08_tf);
        u8g2.drawStr(2+53,screeny - 93,String("Oil Pres").c_str());
        u8g2.drawStr(screenx/2 - 35,screeny - 77, String("psi").c_str());
        u8g2.setFont(u8g2_font_VCR_OSD_mf);
        int x0 = (screenx/2 - 20)-fontx*(9/2);
        int y0 = screeny/2;
        const char *fuelStr = String(value).c_str();
        if (value>=10) {
            u8g2.drawStr(x0 + 5,y0 - 11,fuelStr);
        } 
        else {
        u8g2.drawStr(x0+ 5 + fontx,y0 - 11,fuelStr);
        }
    }
}

void Display::drawGear(const char gear){
    int x0 = screenx/2;
    int y0 = screeny/2;
    clearBox(x0-9,y0-41,30,47);
    u8g2.setFont(u8g2_font_logisoso46_tf);
    u8g2.drawStr(x0 - 10,y0 + 5,String(gear).c_str());
}

void Display::drawMph(const unsigned char mph) {
    int x0 = screenx/2;
    int y0 = screeny/2;
    clearBox(x0 - 11,y0 + 10,32,25);
    u8g2.setFont(u8g2_font_logisoso24_tr);
    if (mph<10) {
        u8g2.drawStr(x0 - 4,y0 + 34, String(mph).c_str());
    } else {
        u8g2.drawStr(x0 - 12,y0 + 34, String(mph).c_str());
    }
    u8g2.sendBuffer();
}

void Display::drawLapTime(int *lapTime){
    u8g2.setFont(u8g2_font_VCR_OSD_mf);

    for (int i = 0; i < 1; ++i) {
        clearBox(2+i*20+fontx+5,109 - fonty,fontx*2,fonty);
    }

    if (lapTime[0]<10) {
        u8g2.drawStr(2 + fontx + 5,109,"0");
        u8g2.drawStr(2 + 2*fontx + 5,109,String(lapTime[0]).c_str());
    }
    else {
        u8g2.drawStr(2 + fontx + 5,109,String(lapTime[0]).c_str());
    }
    if (lapTime[1]<10) {
        u8g2.drawStr(2 + fontx*5 - 5,109,"0");
        u8g2.drawStr(2 + fontx*6 - 5,109,String(lapTime[1]).c_str());
    }
    else {
        u8g2.drawStr(2+ fontx*5 - 5,109,String(lapTime[1]).c_str());
    }
}

void Display::drawBoxGauge(const unsigned int current, const unsigned int max, const unsigned int cutoff, const unsigned int redLine) {
    int padding = 1;
    int xStart = padding;
    int yStart = padding;
    int width = screenx-3*padding + 2;
    int height = screeny - 108;


    // changes the bar
    u8g2.setDrawColor(0);
    u8g2.drawBox(xStart,yStart,screenx-2,height);
    u8g2.setDrawColor(1);

    //  draws rpm on the top right corner
    u8g2.setFont(u8g2_font_VCR_OSD_mf);
    clearBox(190, 50-fonty, fontx*4, fonty);
    char buffer[6];
    if(current < 10000) snprintf(buffer,6," %.1lf", (double)current/1000);
    else snprintf(buffer,6,"%.1lf", (double)current/1000);
    u8g2.drawStr(180, 50, buffer);


    // gauge calculation based on current rpm, how fast the bar moves, faster in the lower rpm and slower at high
    unsigned int cutoffWidth = int(width*(float(cutoff/2.0)/float(max)));

    if (current <= cutoff) {
        u8g2.drawBox(xStart,yStart,int(width*(float(current/2.0)/float(max))),height);
    } else {
        u8g2.drawBox(xStart,yStart,cutoffWidth,height);
        u8g2.drawBox(xStart+cutoffWidth,yStart,int((width-cutoffWidth)*(float(current - cutoff)/float(max - cutoff))),height);
    }
    for (unsigned int i = 0; i<=max; i+=1000) {
        int offset;
        if (i<=cutoff) {
            offset=int((float(i)/float(max))*width)/2;
        } else {
            offset=cutoffWidth+int((float(i-cutoff)/float(max-cutoff))*(width-cutoffWidth));
        }
        if (i==redLine) {
            u8g2.drawVLine(xStart + offset, yStart, height);
        }
        if (i%2000!=0 && i<=cutoff) {
            continue;
        }
        u8g2.setFont(u8g2_font_bitcasual_tn);
    }
    u8g2.setFont(u8g2_font_logisoso18_tf);
    int newFontx = 11;
    int yOff = 2;
    int xOff = 2;
    if (current < redLine) {
        clearBox(screenx/2+25,screeny/2+29-32,screenx/2,33);
    }
    else{   //  code for the shift warning after the redLine limit
        u8g2.drawStr(xStart+width+newFontx+xOff,yStart+height-yOff,String(current/1000).c_str());
        u8g2.drawStr(xStart+width+xOff,yStart+height-yOff,String(current/1000).c_str());
        u8g2.setFont(u8g2_font_logisoso32_tf);
        u8g2.drawStr(screenx/2+25,screeny/2+29,"SHIFT");
        u8g2.setFont(u8g2_font_logisoso18_tf);
    }
    u8g2.sendBuffer();
}