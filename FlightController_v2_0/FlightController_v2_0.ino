#include "FC_MainCommunication.h"
/*
    Name:       FlightController_v2_0.ino
    Created:	30/05/2019 22:12:18
    Author:     Jan Wielgus
	
	Source code of second version of the FlightController on the STM BluePill microcontroller.
*/


#include <FC_Tasker.h>
#include <MyPID.h>
#include <FC_Communication_Base.h>
#include <FC_MPU6050Lib.h>
#include <FC_HMC5883L_Lib.h>
#include <FC_CustomDataTypes.h>
#include <FC_EVA_Filter.h>
#include <FC_Motors.h>



// Functions run by Tasker
void stabilize();
void updateMainCommunication();


// create the tasker
FC_SimpleTasker tasker;

FC_MainCommunication com(&Serial1, 45)



void setup()
{
	// Add functions to the Tasker tasks
	tasker.addFunction(stabilize, 4000L, 15);                  // 250Hz
	tasker.addFunction(updateMainCommunication, 40000L, 15);   // 25Hz
}



void loop()
{
	tasker.runTasker();
}



void stabilize()
{
	
}


void updateMainCommunication()
{
	
}
