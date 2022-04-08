#include "debug_screen.h"
#include "gps.h"
#include "ACAN.h"
#include <ADC.h>
#include <ADC_util.h>
unsigned long lastspeedUpdate;
unsigned long lastRPMUpdate;
unsigned long lastCTempUpdate;
unsigned long lastOTempUpdate;
unsigned long lastVoltUpdate;
unsigned long lastoil_pressureUpdate;
unsigned long lastLapTimeUpdate;
unsigned long lapTimeStart;
unsigned long lastGearUpdate;
unsigned long lastTimeUpdate;
unsigned long lastMainCurrentUpdate;
unsigned long lastFuelCurrentUpdate;
unsigned long lastH20CurrentUpdate;
unsigned long lastFanCurrentUpdate;


//----------------- CAN -----------------//
//static CAN_message_t frame; //FOR CAN Broadcasting if needed
//ID: 0x5F0
unsigned int RPM = 0;
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


ADC* adc = new ADC();

//  Function Prototypes
void setup(Display& driver);
void loop(Display& driver);
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

int main(){
  Display driver = Display();
  //Debug_screen debug = Debug_screen();
  setup(driver);
  while(true){
    loop(driver);
  }

  return 0;
}

void setup(Display& driver) {
  settings.mListenOnlyMode = true;
  const uint32_t errorCode = ACAN::can0.begin (settings, primaryFilters, 12) ;
  if (0 == errorCode) {
    Serial.println ("can0 ok") ;
  }
  else{
    Serial.print ("Error can0: ") ;
    Serial.println (errorCode) ;
  }

  adc->adc0->setAveraging(1); // set number of averages
  adc->adc0->setResolution(8); // set bits of resolution
  adc->adc0->setReference(ADC_REFERENCE::REF_3V3);
  adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED); // change the conversion speed
  adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::MED_SPEED); // change the sampling speed
  adc->adc0->startContinuous(A9);
  adc->adc0->wait_for_cal();

  driver.initializeDisplay();
  Serial.begin(9600);
  Serial.println("test");
  pinMode(LED_BUILTIN,OUTPUT);
  // lastspeedUpdate = 0;
  // lastRPMUpdate = 0;
  // lastCTempUpdate = 0;
  // lastOTempUpdate = 0;
  // lastVoltUpdate = 0;
  // lastoil_pressureUpdate = 0;
  // lastLapTimeUpdate = 0;
  // lastGearUpdate = 0;
  lapTimeStart = millis();
}

void loop(Display& driver) {

  ACAN::can0.dispatchReceivedMessage();
  
  driver.drawBoxGauge(RPM, 12000,cutoff,10000);

  if(coolant_temp >= 100){
    isWorking[0] = false;
    driver.functioning(coolant_temp, cT, false);
  }
  else{
    isWorking[0] = true;
    driver.functioning(coolant_temp, cT, true);
  }
}

static void handleMessage_0 (const CANMessage & frame) {
  RPM = 0.39063 * long((256*long(frame.data[0]) + frame.data[1]));
  throttle = 0.0015259 * long((256*long(frame.data[4]) + frame.data[5]));
  coolant_temp = frame.data[7];
}
