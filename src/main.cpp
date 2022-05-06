#include <display.h>
#include "globalData.h"
#include <SCRLOGGER.hpp>
#include <SCRCAN.hpp>
#include <ADC.h>
#include <ADC_util.h>
#include <SPI.h>

//  Test Variables
int cutoff = 6000;
int lapTime[3] = {0,0,0};
char gears[6] = {'N','1','2','3','4','5'};

// IMPORTANT objects
Display driver = Display(); // Driver Display

// scheduling variables
unsigned long lastRpmUpdate = 0;
unsigned long lastCTempUpdate = 0;
unsigned long lastOTempUpdate = 0;
unsigned long lastOpressureUpdate = 0;
unsigned long lastVoltUpdate = 0;

// IMPORTANT data structure to store logging info


void setup() {
  driver.initializeDisplay();
  initializeSD();
  Serial.begin(9600);
  SPI.setMOSI(11); 
  SPI.setMISO(12); 
  SPI.setSCK(13);
  driver.u8g2.sendBuffer();
}

void loop() {

  SCRCAN::getMessage(); // receives CAN

  //  assigning CAN values to stored variables makes sure read and display values are equal
  current_throttle = SCRCAN::throttle;
  current_coolantTemp = SCRCAN::coolant_temp;
  current_oilTemp = SCRCAN::oil_temp;
  current_oilPressure = SCRCAN::oil_pressure;
  current_battery = SCRCAN::voltage;

  logData();

  // only updates if theres a difference, so bus wont get cluttered
  if(current_throttle != last_throttle){
    driver.drawMph(current_throttle);
    last_throttle = current_throttle;
    driver.u8g2.updateDisplay();
  }

  // UPDATES COOLANT TEMPERATURE
  if(millis() - lastCTempUpdate > 1000 && current_coolantTemp != last_coolantTemp){
    
    if(current_coolantTemp < 100) driver.display_coolantTemp(current_coolantTemp, true);
    else driver.display_coolantTemp(current_coolantTemp, false);
    
    last_coolantTemp = current_coolantTemp;
    lastCTempUpdate = millis();
    driver.u8g2.updateDisplay();
  }

  // UPDATES OIL TEMPERATURE 
  if(millis() - lastOTempUpdate > 1500 && current_oilTemp != last_oilTemp){
    
    if(current_oilTemp < 100) driver.display_oilTemp(current_oilTemp, true);
    else driver.display_oilTemp(current_oilTemp, false);
    
    last_oilTemp = current_oilTemp;
    lastOTempUpdate = millis();
    driver.u8g2.updateDisplay();
  }

  // UPDATES OIL PRESSURE
  if(millis() - lastOpressureUpdate > 1500 && current_oilPressure != last_oilPressure){
    
    if(current_oilPressure < 90 && current_oilPressure >= 30) driver.display_oilPressure(current_oilPressure, true);
    else driver.display_oilPressure(current_oilPressure, false);
    
    last_oilPressure= current_oilPressure;
    lastOpressureUpdate = millis();
    driver.u8g2.updateDisplay();
  }

  // UPDATES BATTERY VOLTAGE
  if(millis() - lastVoltUpdate > 2000 && current_battery != last_battery){
    
    if(current_battery > 10.5) driver.display_batteryVoltage(current_battery, true);
    else driver.display_batteryVoltage(current_battery, false);
    
    last_battery= current_battery;
    lastVoltUpdate = millis();
    driver.u8g2.updateDisplay();
  }
}