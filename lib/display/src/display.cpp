#include "display.h"
#include <string>

Display::Display() :
    u8g2(U8G2_R2, 2, 14, 7, 8, 6, 20, 21, 5, /*enable/wr=*/ 27 , /*cs/ce=*/ 26, /*dc=*/ 25, /*reset=*/24) // Connect RD (orange) with +5V, FS0 and FS1 with GND
{
}

void Display::initializeDisplay()
{   
    // makes the teensy connect with the display
    pinMode(23,OUTPUT);
    analogWrite(23,100);
    u8g2.setBusClock(8000000);
    u8g2.begin();   //  starts the u8g2 object
    u8g2.setContrast(255);
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvR12_tf);
    u8g2.drawStr(32,18,"Driver Booting");
    u8g2.sendBuffer();
    delay(3000);
    drawBackground();   //  sets up the screen of the display
    drawBoxGauge(0, 12000, 6000,10000);
    drawGear('N');
}

//  clears a box making background white and text black
void Display::clearBox(int x0, int y0, int w, int h) {
    u8g2.setDrawColor(0);
    u8g2.drawBox(x0,y0,w,h);
    u8g2.setDrawColor(1);
}

//  fills a block making background black and text white
void Display::drawDarkBox(int x0, int y0, int w, int h) {
    u8g2.setDrawColor(1);
    u8g2.drawBox(x0,y0,w,h);
    u8g2.setDrawColor(0);
}

//  draws the frames and text that are present on the display, runs only once
void Display::drawBackground() {
    int fontx = 12;
    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.drawStr(120 - 5,64 + 43, "mph");

    u8g2.drawFrame(120 - 16,64 + 7,40,40); // mph
    u8g2.drawFrame(120 - 16,64 - 43,40,51); // gear
    u8g2.drawFrame(0,0,240,22); // gauge
    u8g2.drawLine(0,128 - 74,104,128 - 74); // left first horizontal line
    u8g2.drawLine(0,128 - 42,104,128 - 42); // left second horizontal line
    u8g2.drawLine(0,110,104,110); // left third horizontal line
    u8g2.drawLine(140,110,240,110); // right bottom hozizontal line
    u8g2.drawVLine(53,20,66); // left vertical line
    u8g2.setFont(u8g2_font_logisoso16_tr);
    u8g2.drawStr(147, 50, "RPM");

    u8g2.setFont(u8g2_font_VCR_OSD_mf);
    u8g2.drawStr(2 + fontx*4-5,109,":");

}

//  function that changes the coolant temperature value
void Display::display_coolantTemp(const unsigned char value, const bool isFunc){
    if(isFunc) clearBox(0,22,53,32);
    else drawDarkBox(0,22,53,32);

    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.drawStr(2,35,String("Coolant").c_str());
    u8g2.setFont(u8g2_font_VCR_OSD_mf);
    u8g2.drawStr(fontx*3,53,"C");

    int x0 = (value>=100) ? 0 : fontx;
    u8g2.drawStr(x0,53, String(value).c_str());
}

//  function that changes the oil temperature value
void Display::display_oilTemp(const unsigned char value, const bool isFunc){
    if(isFunc) clearBox(0,55,53,31);
    else drawDarkBox(0,55,53,31);

    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.drawStr(2,65,String("Oil").c_str());
    u8g2.setFont(u8g2_font_VCR_OSD_mf);
    u8g2.drawStr(fontx*3,85,"C");

    int x0 = (value>=100) ? 0 : fontx;
    u8g2.drawStr(x0,85,String(value).c_str());
}

//  function that changes the oil pressure value
void Display::display_oilPressure(const unsigned char value, const bool isFunc){
    if(isFunc) clearBox(54,22,50,32);
    else drawDarkBox(54,22,50,32);

    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.drawStr(55,35,String("Oil Pres").c_str());
    u8g2.drawStr(85,51, String("psi").c_str());
    u8g2.setFont(u8g2_font_VCR_OSD_mf);

    int x0 = 100-fontx*(9/2);

    if (value>=10)u8g2.drawStr(x0 + 5,53,String(value).c_str());
    else u8g2.drawStr(x0+4+fontx,53,String(value).c_str());
}

//  function that changes the battery voltage value
void Display::display_batteryVoltage(const float value, const bool isFunc){
    if(isFunc) clearBox(54,55,50,31);
    else drawDarkBox(54,55,50,31); // use set draw color

    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.drawStr(55,65,String("Battery").c_str());
    u8g2.setFont(u8g2_font_VCR_OSD_mf);
    u8g2.setFontMode(1);

    if(value < 10) u8g2.drawStr(56 + fontx,85,String(int(value)).c_str());
    else u8g2.drawStr(56,85,String(int(value)).c_str());

    int firstDecimal = int(value*10.0) % 10;
    u8g2.drawStr(85, 85,String(firstDecimal).c_str());
    u8g2.drawStr(56+fontx*2-fontx/4,85,".");
    u8g2.setFontMode(0);
}

//  function that changes the gear value on the dash
void Display::drawGear(const char gear){
    clearBox(120-9,64-41,30,47);
    u8g2.setFont(u8g2_font_logisoso46_tf);
    u8g2.drawStr(120 - 10,64 + 5,String(gear).c_str());
}

//  function that changes the speed on the dash
void Display::drawMph(const unsigned char mph) {
    clearBox(120 - 11,64 + 10,32,25);
    u8g2.setFont(u8g2_font_logisoso24_tr);
    if (mph<10) {
        u8g2.drawStr(120 - 4,64 + 34, String(mph).c_str());
    } else {
        u8g2.drawStr(120 - 12,64 + 34, String(mph).c_str());
    }
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

//  function that changes value of the rpm on the display
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
}