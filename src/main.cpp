#include "display.h"
#include "ACAN.h"
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


//----------------- CAN -----------------//
//static CAN_message_t frame; //FOR CAN Broadcasting if needed
//ID: 0x5F0
unsigned int RPM = 5000;
unsigned char speed = 0;
unsigned char throttle = 0;

//ID: 0x5F1

//ID: 0x5F2
unsigned char oil_temp = 105;

//ID: 0x5F3
unsigned char oil_pressure = 70;
unsigned char coolant_temp = 105;

//ID: 0x5F4
unsigned int gear;
unsigned int b_voltage;

//ID: 0x5F5

//ID: 0x5F6

//ID: 0x5F7 

//ID: 0x5F8

//  Test Variables
bool isUp = true;
int cutoff = 6000;
int lapTime[3] = {0,0,0};
double voltage = 0.0;
char gears[6] = {'N','1','2','3','4','5'};
bool isWorking[4] = {true,true,true,true}; // coolant, battery, oil temp, oil pressure in this order
char main_c = 0;
Display driver = Display();


ADC* adc = new ADC();

//  Function Prototypes
static void handleMessage_0 (const CANMessage & frame);
static void handleMessage_1 (const CANMessage & frame);
static void handleMessage_2 (const CANMessage & frame);
static void handleMessage_3 (const CANMessage & frame);
static void handleMessage_4 (const CANMessage & frame);
static void handleMessage_5 (const CANMessage & frame);
static void handleMessage_6 (const CANMessage & frame);
static void handleMessage_7 (const CANMessage & frame);
static void handleMessage_8 (const CANMessage & frame);
static void handleMessage_9 (const CANMessage & frame);
static void handleMessage_10 (const CANMessage & frame);
static void handleMessage_11 (const CANMessage & frame);


void setup() {
  
  driver.initializeDisplay();
  Serial.begin(9600);
  Serial.println("Entered Setup");

  ACANSettings settings (500*1000); 
  const ACANPrimaryFilter primaryFilters [] = {
      ACANPrimaryFilter (kData, kExtended, 0x01F0A000, handleMessage_0),// 0x01F0A000 0xF88A000
      ACANPrimaryFilter (kData, kExtended, 0x01F0A003, handleMessage_1), 
      ACANPrimaryFilter (kData, kExtended, 0x01F0A004, handleMessage_2), //oil pressure
      ACANPrimaryFilter (kData, kExtended, 0x01F0A005, handleMessage_3), //launch active (laungh ramp time?)
      ACANPrimaryFilter (kData, kExtended, 0x01F0A007, handleMessage_4), //oil temp logging
      ACANPrimaryFilter (kData, kExtended, 0x01F0A008, handleMessage_5), //launch rpm fuel cut
      ACANPrimaryFilter (kData, kExtended, 0x01F0A010, handleMessage_6), //Sparkcut fuelcut
      ACANPrimaryFilter (kData, kExtended, 0x01F0A012, handleMessage_7), //tc_slip measured
      ACANPrimaryFilter (kData, kExtended, 0x0000A0000, handleMessage_8),
      ACANPrimaryFilter (kData, kExtended, 0x0000A0001, handleMessage_9),
      ACANPrimaryFilter (kData, kExtended, 0x0000A0003, handleMessage_10),
      ACANPrimaryFilter (kData, kExtended, 0x0000A0004, handleMessage_11)
  };
  Serial.println("ACANSettings done");

  const uint32_t errorCode = ACAN::can0.begin (settings, primaryFilters, 12) ;
  if (0 == errorCode) {
    Serial.println ("can0 ok") ;
  }
  else{
    Serial.print ("Error can0: ") ;
    Serial.println (errorCode) ;
  }

  Serial.println("End Setup");
}

void loop() {

  ACAN::can0.dispatchReceivedMessage();
  
  driver.drawBoxGauge(throttle * 50, 12000,cutoff,10000);

  driver.display_coolantTemp(throttle, false);

  driver.display_oilTemp(throttle, false);

  driver.display_oilPressure(throttle, false);

  driver.drawMph(throttle);

  driver.drawGear(throttle/10);

  driver.display_batteryVoltage(throttle, false);

  // Serial.println(throttle);
}

//----------------- CAN Handling -----------------//
static void handleMessage_0 (const CANMessage & frame) {
  RPM = 0.39063 * long((256*long(frame.data[0]) + frame.data[1]));
  throttle = 0.0015259 * long((256*long(frame.data[4]) + frame.data[5]));
  coolant_temp = frame.data[7];
}
static void handleMessage_1 (const CANMessage & frame) {
  // converted from kph to mph
  speed = 0.00390625 * long((256*long(frame.data[2]) + frame.data[3]));
  gear = frame.data[4];
  voltage = 0.0002455 * long((256*long(frame.data[6]) + frame.data[7]));
}
static void handleMessage_2 (const CANMessage & frame) {
  //fuel_pressure = 0.580151 * frame.data[3];
  oil_pressure = 0.580151 * frame.data[4];
  //VE = frame.data[2];
}
static void handleMessage_3 (const CANMessage & frame) {
  //launch_active = frame.data[8]; // Check, its bit 1 of byte 7 in the frame.
}
static void handleMessage_4 (const CANMessage & frame) {
  oil_temp = frame.data[4] - 50;
  //logging  = frame.data[8];  // Check, its bit 1 of byte 7 in the frame.
}
static void handleMessage_5 (const CANMessage & frame) {
  // launch_rpm = 0.39063 * long((256*long(frame.data[3]) + frame.data[4]));
      
  // if(frame.data[7] == 0){
  //     error = 0;
  // }
  // else{
  //     error = 1;
  // }
}
static void handleMessage_6 (const CANMessage & frame) {
  // TC_FuelCut = frame.data[0] * 0.392157; //% Fuel Cut
  // TC_SparkCut = frame.data[1] * 0.392157;//% Spark Cut
  // TC_Mode = frame.data[4]; //TC Strength
}
static void handleMessage_7 (const CANMessage & frame) {
  //converted from kph to mph
  // traction_control = 0.01242742 * long((256*long(frame.data[0]) + frame.data[1]));
  // TC_SlipMeas = 0.01242742 * long((256*long(frame.data[2]) + frame.data[3])); //0 - 1310.7 kph
}
static void handleMessage_8 (const CANMessage & frame) {
  // gps_lat = (frame.data[0]-2147483647.5)*4.19095159*pow(10,-8);
  // gps_long = (frame.data[4]-2147483647.5)*8.38190317*pow(10,-8); // assuming deg range uses all 32 bits
}
static void handleMessage_9 (const CANMessage & frame) {
  // gps_speed = 0.01 * long((256*long(frame.data[0]) + frame.data[1]));
  // gps_altitude = long((256*long(frame.data[2]) + frame.data[3])); 
  // this is signed, not sure how the library converts it; it is signed by magnitude, not 2's complement
}
static void handleMessage_10 (const CANMessage & frame) {
  // all 16 bit signed... 
  // x_acceleration = 0.000244141 * long((256*long(frame.data[0]) + frame.data[1])); 
  // y_acceleration = 0.000244141 * long((256*long(frame.data[2]) + frame.data[3])); 
  // z_acceleration = 0.000244141 * long((256*long(frame.data[4]) + frame.data[5])); 
}
static void handleMessage_11 (const CANMessage & frame) {
  // x_yaw = 0.015258789 * long((256*long(frame.data[0]) + frame.data[1])); 
  // y_yaw = 0.015258789 * long((256*long(frame.data[2]) + frame.data[3])); 
  // z_yaw = 0.015258789 * long((256*long(frame.data[4]) + frame.data[5])); 
}
