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
#include "config.h"



// create the tasker object
FC_SimpleTasker tasker;

// create the communication object
FC_MainCommunication com(&Serial1, 45);

// create sensors objects
FC_MPU6050Lib mpu;
FC_HMC5883L_Lib compass;
// Baro object is created indeide the library



// create motors object
FC_Motors motors;

// PID objects
static float calculationsDeltaTime = 0.004f; //  = 1/250
MyPID levelXpid(calculationsDeltaTime, 0, 0, 0, 100);
MyPID levelYpid(calculationsDeltaTime, 0, 0, 0, 100);
MyPID yawPID(calculationsDeltaTime, 0, 0, 0, 70);
MyPID altHoldPID(calculationsDeltaTime, 0, 0, 0, 50);




// variables
FC_MPU6050Lib::vector3Float angle; // X and Y angles
float heading;
float headingToHold = 0; // calculated value based on the pilot rotate stick to hold by the drone
float pressureToHold;

// extrapolation variables
bool needToExtrapolateStickVlauesFlag = false;
int16_t previousTBvalue = 0;
int16_t previousLRvalue = 0;
float extrapolatedTBstick = 0;
float extrapolatedLRstick = 0;
FC_EVA_Filter tbFilter(0.4);
FC_EVA_Filter lrFilter(0.4);


// flight modes
enum FlightMode { STABILIZE = 0, ALT_HOLD = 1, POS_HOLD = 2 }; // list of all flight modes (special flight modes in the different emum)
//enum SpecialFlightModes {LANDING=3, RETURN_TO_LAUNCH=4, RETURN_OVER_PILOT=5};
bool needToUpdateAltHoldPID_flag = false;
int16_t pidXval, pidYval, pidYawVal, pidAltHoldVal; // PID controllers results



// typedefs (not necessary to compile)
typedef uint8 uint8_t;
typedef int8 int8_t;
typedef uint16 uint16_t;
typedef int16 int16_t;



#endif /* STORAGE_H_ */

