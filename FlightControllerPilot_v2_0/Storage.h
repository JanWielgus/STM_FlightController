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


// Include files
	#include <FC_Tasker.h>
	#include "FC_MainCommunication.h"
	#include "FC_ControlStick.h"
	#include <LiquidCrystal_I2C.h>
	#include <SoftwareSerial.h>
	#include "config.h"




// Objects
	FC_SimpleTasker tasker;

	FC_MainCommunication com(&Serial, 100);

	LiquidCrystal_I2C lcd(config::LCD_ADDRESS, 16, 2);
	
	SoftwareSerial bluetoothSoftwareSerial(config::pin.btRX, config::pin.btTX); // RX, TX
	
	// control sticks
	FC_ControlStick thrStick;
	FC_ControlStick rotStick;
	FC_ControlStick TB_Stick;
	FC_ControlStick LR_Stick;




// Other types
	enum stateType {disarmed, arming1, arming2, armed};
	stateType state = disarmed;
	
	
	
// Bluetooth app received data
	bool btNeedToUpdatePIDFlag = false; // if this is true - need to send new PID data to the drone
	// RESET THIS TO false AFTER SENDING TO THE DRONE
	
	uint8_t btControllerID;
	float btPID_P;
	float btPID_I;
	uint8_t btPID_Imax;
	float btPID_D;



#endif /* STORAGE_H_ */