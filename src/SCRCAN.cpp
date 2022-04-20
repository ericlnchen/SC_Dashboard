#include "SCRCAN.hpp"

namespace SCRCAN {

    int RPM;
    unsigned char throttle = 0;
    signed char coolant_temp;

    // ID: 0x01F0A003
    unsigned short int speed;
    unsigned char gear;
    float voltage;

    // ID: 0x01F0A004
    float fuel_pressure;
    float oil_pressure;
    int VE;

    // ID: 0x01F0A005
    bool launch_active; // Clutch Sw

    // ID: 0x01F0A007
    int oil_temp; // offset -50C
    bool logging; // logging active ? T/F

    // ID: 0x01F0A008
    int launch_rpm; // 2StepTargetFuel [RPM]
    int error;      // if any errors are a 1, then error = 1

    int TC_FuelCut;  //% Fuel Cut
    int TC_SparkCut; //% Spark Cut
    int TC_Mode;     // TC Strength

    // ID: 0x01F0A012
    int traction_control; // TC_SlipTarget; 16 bit unsigned 0.02 kph/bit 0-1310.7 kph
    int TC_SlipMeas;

    // ID: 0x0000A0000
    float gps_lat;
    float gps_long;

    // ID: 0x0000A0001
    int gps_speed;
    int gps_altitude;

    // ID: 0x0000A0003
    int x_acceleration;
    int y_acceleration;
    int z_acceleration;

    // ID: 0x0000A0004
    int x_yaw;
    int y_yaw;
    int z_yaw;

    //----------------- CAN Handling -----------------//

    void can_init(){
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

        const uint32_t errorCode = ACAN::can0.begin (settings, primaryFilters, 12);
        if (0 == errorCode) {
            Serial.println ("can0 ok");
        }
        else{
            Serial.print ("Error can0: ");
            Serial.println (errorCode);
        }

        Serial.println("End Setup");
    }

    void getMessage(){
        ACAN::can0.dispatchReceivedMessage();
    }

    void handleMessage_0(const CANMessage &frame)
    {
        Serial.println("Read throttle");
        RPM = 0.39063 * long((256 * long(frame.data[0]) + frame.data[1]));
        throttle = 0.0015259 * long((256 * long(frame.data[4]) + frame.data[5]));
        coolant_temp = frame.data[7];
    }
    void handleMessage_1(const CANMessage &frame)
    {
        Serial.println("Read voltage");
        // converted from kph to mph
        speed = 0.00390625 * long((256 * long(frame.data[2]) + frame.data[3]));
        gear = frame.data[4];
        voltage = 0.0002455 * long((256 * long(frame.data[6]) + frame.data[7]));
        Serial.print(voltage);
    }
    void handleMessage_2(const CANMessage &frame)
    {
        Serial.println("Read fuel/oil");
        fuel_pressure = 0.580151 * frame.data[3];
        oil_pressure = 0.580151 * frame.data[4];
        VE = frame.data[2];
    }
    void handleMessage_3(const CANMessage &frame)
    {
        Serial.println("Read launch ctrl");
        launch_active = frame.data[8]; // Check, its bit 1 of byte 7 in the frame.
    }
    void handleMessage_4(const CANMessage &frame)
    {
        Serial.print("Read oil temp");
        oil_temp = frame.data[4] - 50;
        logging = frame.data[8]; // Check, its bit 1 of byte 7 in the frame.
    }
    void handleMessage_5(const CANMessage &frame)
    {
        Serial.println("Read launch rpm");
        launch_rpm = 0.39063 * long((256 * long(frame.data[3]) + frame.data[4]));

        if (frame.data[7] == 0)
        {
        error = 0;
        }
        else
        {
        error = 1;
        }
    }
    void handleMessage_6(const CANMessage &frame)
    {
        Serial.println("Fuel cut");
        TC_FuelCut = frame.data[0] * 0.392157;  //% Fuel Cut
        TC_SparkCut = frame.data[1] * 0.392157; //% Spark Cut
        TC_Mode = frame.data[4];                // TC Strength
    }
    void handleMessage_7(const CANMessage &frame)
    {
        // converted from kph to mph
        traction_control = 0.01242742 * long((256 * long(frame.data[0]) + frame.data[1]));
        TC_SlipMeas = 0.01242742 * long((256 * long(frame.data[2]) + frame.data[3])); // 0 - 1310.7 kph
    }
    void handleMessage_8(const CANMessage &frame)
    {
        gps_lat = (frame.data[0] - 2147483647.5) * 4.19095159 * pow(10, -8);
        gps_long = (frame.data[4] - 2147483647.5) * 8.38190317 * pow(10, -8); // assuming deg range uses all 32 bits
    }
    void handleMessage_9(const CANMessage &frame)
    {
        gps_speed = 0.01 * long((256 * long(frame.data[0]) + frame.data[1]));
        gps_altitude = long((256 * long(frame.data[2]) + frame.data[3]));
        // this is signed, not sure how the library converts it; it is signed by magnitude, not 2's complement
    }
    void handleMessage_10(const CANMessage &frame)
    {
        // all 16 bit signed...
        x_acceleration = 0.000244141 * long((256 * long(frame.data[0]) + frame.data[1]));
        y_acceleration = 0.000244141 * long((256 * long(frame.data[2]) + frame.data[3]));
        z_acceleration = 0.000244141 * long((256 * long(frame.data[4]) + frame.data[5]));
    }
    void handleMessage_11(const CANMessage &frame)
    {
        x_yaw = 0.015258789 * long((256 * long(frame.data[0]) + frame.data[1]));
        y_yaw = 0.015258789 * long((256 * long(frame.data[2]) + frame.data[3]));
        z_yaw = 0.015258789 * long((256 * long(frame.data[4]) + frame.data[5]));
    }
}