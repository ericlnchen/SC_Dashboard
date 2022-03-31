#include "display.h"

class Debug_screen : protected Display
{
public:
    Debug_screen();
    void initialize_screen();
    void drawLeftDiagnostics();
    void drawRightDiagnostics();
private:
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