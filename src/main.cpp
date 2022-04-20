#include "display.h"
#include "SCRCAN.hpp"
#include <ADC.h>
#include <ADC_util.h>

//----------------- PIN DEFS -----------------//
#define LCD1      0  //(RX)
#define LCD2      1  //(TX)
#define LCD3      2  //(IO for interface control)
#define CAN_TX    3 
#define CAN_RX    4
#define STBY      5  // CAN FREE THIS PIN BY TYING TO 5V
#define ALT_SW    6  // CONTROLS ALTERNATOR CONN
#define NEUTRAL   7
#define TACH      8  // NEOPIXEL PIN
#define FUEL_FUSE 9  // DIGITAL CONTROL PIN OF FUEL PUMP
#define SD_CS     10 // SPI PINS
#define MOSI      11
#define MISO      12
#define SCK       13
#define CURR_H2O  14 // ANALOG INPUTS FROM CURRENT SENSORS
#define CURR_FUEL 15
#define CURR_FAN  16
#define CURR_12V  17
#define VOLTMETER 18 // Redundant AEM HAS THIS, REMOVE
#define ALT_CURR  19
#define V_FUSE  20   // DIGITAL CONTROL PIN OF LOADS
#define H2O_FUSE  21
#define FAN_FUSE  22
//                23  | N/U       <- OPEN
//                A14 | N/U       <- DAC (not used, use as a digital pin if possible)

//  Test Variables
// bool isUp = true;
int cutoff = 6000;
// int lapTime[3] = {0,0,0};
// double voltage = 0.0;
// char gears[6] = {'N','1','2','3','4','5'};
// bool isWorking[4] = {true,true,true,true}; // coolant, battery, oil temp, oil pressure in this order
// char main_c = 0;
Display driver = Display();


ADC* adc = new ADC();

void setup() {
  
  driver.initializeDisplay();
  Serial.begin(9600);
  Serial.println("Entered Setup");

  SCRCAN::can_init();
}

void loop() {

  SCRCAN::getMessage();
  
  driver.drawBoxGauge(SCRCAN::throttle * 50, 12000,cutoff,10000);

  driver.display_coolantTemp(SCRCAN::throttle, false);

  driver.display_oilTemp(SCRCAN::throttle, false);

  driver.display_oilPressure(SCRCAN::throttle, false);

  driver.drawMph(SCRCAN::throttle);

  driver.drawGear(SCRCAN::throttle/10);

  driver.display_batteryVoltage(SCRCAN::throttle, false);
}