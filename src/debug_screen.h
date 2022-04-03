#include "display.h"

class Debug_screen : public Display
{
public:
    Debug_screen();
    void initializeDisplay();
    void drawBackground();
    void drawLeftDiagnostics(char value, char diag, bool isFunc);
    void drawRightDiagnostics();
    void drawBoxGauge(const unsigned int current, const unsigned int max, const unsigned int cutoff, const unsigned int redLine);
    void functioning_battery(const double value, const bool isFunc);

private:
    int leftBoxHeight = 32;
    int leftBoxLength = 53;
    double main_current;
    double h20_current;
    double fuel_current;
    double fan_current;
    unsigned int RPM;
    unsigned char speed;
    unsigned char oil_temp;
    unsigned char oil_pressure;
    unsigned char coolant_temp;
    unsigned int gear;
    unsigned int b_voltage;
};