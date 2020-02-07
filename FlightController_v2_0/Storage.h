/*
 * Storage.h
 *
 * Created: 21/07/2019 17:45:28
 *  Author: Jan Wielgus
 *
 *
 * There are all global variables and objects
 */ 


#ifndef STORAGE_H_
#define STORAGE_H_


#include <FC_Tasker.h>
#include <MyPID.h>
#include <FC_Communication_Base.h>
#include "FC_MainCommunication.h"
#include <FC_MPU6050Lib.h>
#include <FC_HMC5883L_Lib.h>
#include <FC_MS5611_Lib.h>
#include <FC_EVA_Filter.h>
#include <FC_Motors.h>
#include "DebugSystem.h"



// New data types

    enum FlightModeType { STABILIZE = 0, ALT_HOLD = 1, POS_HOLD = 2 }; // list of all flight modes
    // Do not use separate flight modes type. ADD ALL FLIGHT MODES TO THE ENUM ABOVE !!!
    // {LANDING=3, RETURN_TO_LAUNCH=4, RETURN_OVER_PILOT=5};

    enum BaudRates { BAUD_9600 = 9600, BAUD_19200 = 19200, BAUD_38400 = 38400, BAUD_57600 = 57600, BAUD_115200 = 115200};




// Variables and changing state stuff

namespace Storage
{
    // Flags
    struct
    {
        bool needToExtrapolateStickVlaues = false;
    } flags;



    // Objects
    extern FC_SimpleTasker tasker;
    extern FC_MainCommunication com;
    extern FC_MPU6050Lib mpu;
    extern FC_HMC5883L_Lib compass;
    extern FC_Motors motors;
    extern DebugSystem debug;


    // PID objects
    extern MyPID levelXpid;
    extern MyPID levelYpid;
    extern MyPID yawPID;
    extern MyPID altHoldPID;


    // Variables
    extern FC_MPU6050Lib::vector3Float angle;
    extern float heading;
    extern float headingToHold;
    extern float pressureToHold;


    // Extrapolation variables
    extern int16_t previousTBvalue;
    extern int16_t previousLRvalue;
    extern float extrapolatedTBstick;
    extern float extrapolatedLRstick;
    extern FC_EVA_Filter tbFilter;
    extern FC_EVA_Filter lrFilter;


    // Flight modes
    extern int16_t lastPID_LevelX_value;
    extern int16_t lastPID_LevelY_value;
    extern int16_t lastPID_Yaw_value;
    extern int16_t lastPID_AltHold_value;
}




#endif /* STORAGE_H_ */

