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
#include <FC_MPU6050Lib.h>
#include <FC_HMC5883L_Lib.h>
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
FC_MPU6050Lib::vector3Float angle; // X and Y angles
float heading;

// create motors object
FC_Motors motors;




// variables
float headingToHold = 0; // calculated value based on the pilot rotate stick to hold by the drone



#endif /* STORAGE_H_ */