#include "debug_screen.h"

Debug_screen::Debug_screen() :
    u8g2(U8G2_R2, 2, 14, 7, 8, 6, 20, 21, 5, /*enable/wr=*/ 27 , /*cs/ce=*/ 26, /*dc=*/ 25, /*reset=*/24) // Connect RD (orange) with +5V, FS0 and FS1 with GND
{
}

void Debug_screen::initializeDisplay(){
    pinMode(23,OUTPUT);
    analogWrite(23,100);
    u8g2.begin();
    u8g2.setContrast(255);
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_helvR12_tf);
    u8g2.drawStr(32,18,"Debug Booting");
    u8g2.sendBuffer();
    delay(3000);
    drawBackground();
    drawBoxGauge(0, 12000, 6000,10000);
    u8g2.sendBuffer();
}

void Debug_screen::drawBackground(){

    u8g2.setFont(u8g2_font_logisoso16_tr);
    u8g2.drawStr(147, 50, "RPM");
    u8g2.setFont(u8g2_font_helvR08_tf);
    u8g2.drawStr(screenx/2 - 5,screeny/2 + 43, "mph");
    u8g2.drawFrame(0,0,screenx,22); // gauge
    u8g2.drawLine(0,54,104,54); // left first horizontal line
    u8g2.drawLine(0,88,104,88); // left second horizontal line
    u8g2.drawLine(0,122,104,122); // left third horizontal line
    u8g2.drawLine(105,71,143,71); // middle first horizontal line
    u8g2.drawLine(105,110,143,110); // middle second horizontal line
    u8g2.drawLine(105,122,143,122); // middle third horizontal line
    u8g2.drawVLine(53,20,102); // 1st vertical line
    u8g2.drawVLine(104,20,102); // 2nd vertical line
    u8g2.drawVLine(143,20,102); // 2nd vertical line
}

void Debug_screen::drawBoxGauge(const unsigned int current, const unsigned int max, const unsigned int cutoff, const unsigned int redLine){
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
    }
    u8g2.sendBuffer();
}

// to-do changing the coordinates in clear, drawDark, all draw strings
void Debug_screen::functioning_battery(const double value, const bool isFunc){
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


void Debug_screen::drawLeftDiagnostics(){

}

void Debug_screen::drawRightDiagnostics(){

}