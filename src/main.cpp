#include <display.h>
#include <SCRCAN.hpp>
#include <ADC.h>
#include <ADC_util.h>
#include <SPI.h>

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
int cutoff = 6000;
int lapTime[3] = {0,0,0};
char gears[6] = {'N','1','2','3','4','5'};
Display driver = Display();


// IMPORTANT control variables
int current_throttle;
int last_throttle;
int current_coolantTemp;
int last_coolantTemp;


ADC* adc = new ADC();

void setup() {
  
  driver.initializeDisplay();
  Serial.begin(9600);
  Serial.println("Entered Setup");

    //----------------- Serial Init -----------------//
  SPI.setMOSI(11); 
  SPI.setMISO(12); 
  SPI.setSCK(13);

  SCRCAN::can_init();
}

void loop() {

  SCRCAN::getMessage(); // receives CAN

  //  assigning CAN values to stored variables makes sure read and display values are equal
  current_throttle = SCRCAN::throttle;
  current_coolantTemp = SCRCAN::coolant_temp;

  // only updates if theres a difference, so bus wont get cluttered
  if(current_throttle != last_throttle){
    driver.drawMph(current_throttle);
    last_throttle = current_throttle;
    driver.u8g2.updateDisplay(); // uses update display so screen doesn't always refresh
  }
  if(current_coolantTemp != last_coolantTemp){
    if(current_coolantTemp < 100){
      driver.display_coolantTemp(current_coolantTemp, true);
    }
    else driver.display_coolantTemp(current_coolantTemp, false);
    last_coolantTemp = current_coolantTemp;
    driver.u8g2.updateDisplay(); // uses update display so screen doesn't always refresh
  }
}