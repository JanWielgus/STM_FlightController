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


// Only for STM32 (maybe not necessary)
typedef uint8 uint8_t;
typedef int8 int8_t;
typedef uint16 uint16_t;
typedef int16 int16_t;

enum FlightMode { STABILIZE = 0, ALT_HOLD = 1, POS_HOLD = 2 }; // list of all flight modes (special flight modes in the different emum)
//enum SpecialFlightModes {LANDING=3, RETURN_TO_LAUNCH=4, RETURN_OVER_PILOT=5};


// Flags
struct
{
	bool needToExtrapolateStickVlaues = false;
	bool needToUpdateAltHoldPID = false;
} flags;



// Objects
extern FC_SimpleTasker tasker;
extern FC_MainCommunication com;
extern FC_MPU6050Lib mpu;
extern FC_HMC5883L_Lib compass;
extern FC_Motors motors;

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
extern int16_t pidXval;
extern int16_t pidYval;
extern int16_t pidYawVal;
extern int16_t pidAltHoldVal;

extern uint16_t altHoldBaseThrottle;


#endif /* STORAGE_H_ */

