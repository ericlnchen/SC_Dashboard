#include "debug_screen.h"
#include "gps.h"
#include "ACAN.h"
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


//----------------- CAN -----------------//
//static CAN_message_t frame; //FOR CAN Broadcasting if needed
//ID: 0x5F0
unsigned int RPM = 0;
unsigned char speed = 0;

//ID: 0x5F1

//ID: 0x5F2
unsigned char oil_temp = 105;

//ID: 0x5F3
unsigned char oil_pressure = 70;
unsigned char coolant_temp = 102;

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
double voltage = 11.4;
char gears[6] = {'N','1','2','3','4','5'};
bool isWorking[4] = {true,true,true,true}; // coolant, battery, oil temp, oil pressure in this order

//  Function Prototypes
void setup(Display& driver);
void loop(Display& driver);


int main(){
  //Display driver = Display();
  Debug_screen debug = Debug_screen();
  setup(debug);
  while(true){
    loop(debug);
  }

  return 0;
}

void setup(Display& driver) {
  driver.initializeDisplay();
  Serial.begin(9600);
  Serial.println("test");
  pinMode(LED_BUILTIN,OUTPUT);
  lastspeedUpdate = 0;
  lastRPMUpdate = 0;
  lastCTempUpdate = 0;
  lastOTempUpdate = 0;
  lastVoltUpdate = 0;
  lastoil_pressureUpdate = 0;
  lastLapTimeUpdate = 0;
  lastGearUpdate = 0;
  lapTimeStart = millis();
}

void loop(Display& driver) {
  
  if(RPM == 11500){
    if(gear == 5){
      isUp = false;
    }
    else if(gear == 0){
      isUp = true;
    }
    if(isUp) gear += 1;
    else gear -= 1;
    driver.drawGear(gears[gear]);
    lastGearUpdate = millis();
    RPM = 0;
  }
  else {
   RPM+=100;    
  }
  driver.drawBoxGauge(RPM, 12000,cutoff,10000);
  
  if (millis()-lastspeedUpdate>500){
    if (speed==99) {
      speed = 0;
    } else {
      speed++;
    }
    driver.drawMph(speed);
    lastspeedUpdate = millis();
  }

  // if (millis()-lastLapTimeUpdate>=1000){
  //   int seconds = millis()/1000;
  //   // lapTime[2] = millis();
  //   lapTime[1] = seconds%60;
  //   lapTime[0] = (seconds/60)%60;
  //   driver.drawLapTime(lapTime);
  //   lastLapTimeUpdate = millis();
  // }

  // if (millis()-lastoil_pressureUpdate>3000){
  //   if(oil_pressure == 0){
  //     oil_pressure = 90;
  //   }
  //   if(oil_pressure >= 90){
  //     isWorking[3] = false;
  //     driver.functioning(oil_pressure, oP, false);
  //   }
  //   else if(oil_pressure <= 30){
  //     isWorking[3] = false;
  //     driver.functioning(oil_pressure, oP, false);
  //   }
  //   else{
  //     isWorking[3] = true;
  //     driver.functioning(oil_pressure, oP, true);
  //   }
  //   --oil_pressure;
  //   lastoil_pressureUpdate = millis();
  // }

  // if (millis()-lastVoltUpdate>1000){
  //   voltage=voltage+.1;
  //   if(voltage <= 0.0){
  //     voltage = 12.0;
  //   }
  //   else if(voltage >= 14.5){
  //     voltage = 10;
  //   }
  //   if(voltage <= 11.5){
  //     isWorking[1] = false;
  //     driver.functioning_battery(voltage, false);
  //   }
  //   else{
  //     isWorking[1] = true;
  //     driver.functioning_battery(voltage, true);
  //   }
  //   lastVoltUpdate = millis();
  // }

  // if (millis()-lastCTempUpdate>4000){
  //   if(coolant_temp == 0){
  //     coolant_temp = 99;
  //   }
  //   if(coolant_temp >= 100){
  //     isWorking[0] = false;
  //     driver.functioning(coolant_temp, cT, false);
  //   }
  //   else{
  //     isWorking[0] = true;
  //     driver.functioning(coolant_temp, cT, true);
  //   }
  //    --coolant_temp;
  //   lastCTempUpdate = millis();
  // }

  // if (millis()-lastOTempUpdate>4500){
  //   if(oil_temp == 0){
  //     oil_temp = 99;
  //   }
  //   if(oil_temp >= 100){
  //     isWorking[2] = false;
  //     driver.functioning(oil_temp, oT, false);
  //   }
  //   else{
  //     isWorking[2] = true;
  //     driver.functioning(oil_temp, oT, true);
  //   }
  //   --oil_temp;
  //   lastOTempUpdate = millis();
  // }
}

