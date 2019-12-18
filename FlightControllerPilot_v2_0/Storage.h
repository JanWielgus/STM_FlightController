/*
 * Storage.h
 *
 * Created: 20/07/2019 19:27:18
 *  Author: Jan Wielgus
 *
 *
 * All global variables and objects are created here
 * (everything needed by multiple files)
 */ 


#ifndef STORAGE_H_
#define STORAGE_H_

#include <FC_Tasker.h>
#include "FC_MainCommunication.h"
#include "FC_ControlStick.h"
#include <LiquidCrystal_I2C.h>
#include "LCDhandler.h"
#include <SoftwareSerial.h>
#include "AndroidCommunication.h"
#include "GestureRecognizer.h"


// New data types
enum armStateType { disarmed, arming1, arming2, armed };


// All global flags
struct
{
	bool btNeedToUpdatePID = false; // if this is true - need to send new PID data to the drone
	// RESET THIS TO false AFTER SENDING TO THE DRONE
} flags;


// Objects
extern FC_SimpleTasker tasker;
extern FC_MainCommunication com;
extern LiquidCrystal_I2C lcd;
extern LcdHandler display;
extern SoftwareSerial bluetoothSoftwareSerial;
extern AndroidCommunication androidCom;
extern GestureRecognizer gestureRecognizer;


// control sticks
extern FC_ControlStick thrStick;
extern FC_ControlStick rotStick;
extern FC_ControlStick TB_Stick;
extern FC_ControlStick LR_Stick;


// Other types
extern armStateType armState; // old "state"


struct
{
	// Bluetooth app received data
	uint8_t controllerID;
	float PID_P;
	float PID_I;
	uint8_t PID_Imax;
	float PID_D;
} androidData;



#endif /* STORAGE_H_ */