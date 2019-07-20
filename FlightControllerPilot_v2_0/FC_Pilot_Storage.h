/*
 * FC_Pilot_Storage.h
 *
 * Created: 20/07/2019 19:27:18
 *  Author: Jan Wielgus
 *
 *
 * All global variables and objects are created here
 * (everything needed by multiple files)
 */ 


#ifndef FC_PILOT_STORAGE_H_
#define FC_PILOT_STORAGE_H_


// Include files
	#include <FC_Tasker.h>
	#include "FC_MainCommunication.h"
	#include "FC_ControlStick.h"
	#include <LiquidCrystal_I2C.h>
	#include "config.h"




// Objects
	FC_SimpleTasker tasker;

	FC_MainCommunication com(&Serial, 45);

	LiquidCrystal_I2C lcd(config::LCD_ADDRESS, 16, 2);
	
	// control sticks
	FC_ControlStick thrStick;
	FC_ControlStick rotStick;
	FC_ControlStick TB_Stick;
	FC_ControlStick LR_Stick;




// Other types
	enum stateType {disarmed, arming1, arming2, armed};
	stateType state = disarmed;



#endif /* FC_PILOT_STORAGE_H_ */