#pragma once

#include <Arduino.h>
#include <ACAN.h>

namespace SCRCAN
{
//----------------- CAN -----------------//
  // static CAN_message_t frame; //FOR CAN Broadcasting if needed
  // ID: 0x01F0A000
  extern int RPM;
  extern unsigned char throttle;
  extern signed char coolant_temp;

  // ID: 0x01F0A003
  extern unsigned short int speed;
  extern unsigned char gear;
  extern float voltage;

  // ID: 0x01F0A004
  extern float fuel_pressure;
  extern float oil_pressure;
  extern int VE;

  // ID: 0x01F0A005
  extern bool launch_active; // Clutch Sw

  // ID: 0x01F0A007
  extern int oil_temp; // offset -50C
  extern bool logging; // logging active ? T/F

  // ID: 0x01F0A008
  extern int launch_rpm; // 2StepTargetFuel [RPM]
  extern int error;      // if any errors are a 1, then error = 1

  extern int TC_FuelCut;  //% Fuel Cut
  extern int TC_SparkCut; //% Spark Cut
  extern int TC_Mode;     // TC Strength

  // ID: 0x01F0A012
  extern int traction_control; // TC_SlipTarget; 16 bit unsigned 0.02 kph/bit 0-1310.7 kph
  extern int TC_SlipMeas;

  // ID: 0x0000A0000
  extern float gps_lat;
  extern float gps_long;

  // ID: 0x0000A0001
  extern int gps_speed;
  extern int gps_altitude;

  // ID: 0x0000A0003
  extern int x_acceleration;
  extern int y_acceleration;
  extern int z_acceleration;

  // ID: 0x0000A0004
  extern int x_yaw;
  extern int y_yaw;
  extern int z_yaw;

  //  Function Prototypes for all 12 packets
  void handleMessage_0 (const CANMessage & frame);
  void handleMessage_1 (const CANMessage & frame);
  void handleMessage_2 (const CANMessage & frame);
  void handleMessage_3 (const CANMessage & frame);
  void handleMessage_4 (const CANMessage & frame);
  void handleMessage_5 (const CANMessage & frame);
  void handleMessage_6 (const CANMessage & frame);
  void handleMessage_7 (const CANMessage & frame);
  void handleMessage_8 (const CANMessage & frame);
  void handleMessage_9 (const CANMessage & frame);
  void handleMessage_10 (const CANMessage & frame);
  void handleMessage_11 (const CANMessage & frame);

  /**
   * @brief initialize mcp2561 + CAN transmission
   */
  void can_init();

  /**
   * @brief loop to retrieve CAN data and store
   * 
   */
  void getMessage();
}