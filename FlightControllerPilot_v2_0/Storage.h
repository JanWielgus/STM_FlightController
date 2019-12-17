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
#include <SoftwareSerial.h>


// New data types
enum armStateType { disarmed, arming1, arming2, armed };


// Objects
extern FC_SimpleTasker tasker;
extern FC_MainCommunication com;
extern LiquidCrystal_I2C lcd;
extern SoftwareSerial bluetoothSoftwareSerial;

// control sticks
extern FC_ControlStick thrStick;
extern FC_ControlStick rotStick;
extern FC_ControlStick TB_Stick;
extern FC_ControlStick LR_Stick;

// Other types
extern armStateType armState; // old "state"

// Bluetooth app received data
extern bool btNeedToUpdatePIDFlag;

extern uint8_t btControllerID;
extern float btPID_P;
extern float btPID_I;
extern uint8_t btPID_Imax;
extern float btPID_D;



#endif /* STORAGE_H_ */