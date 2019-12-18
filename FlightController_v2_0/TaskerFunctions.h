// TaskerFunctions.h

#ifndef _TASKERFUNCTIONS_h
#define _TASKERFUNCTIONS_h

void addTaskerFunctionsToTasker();

namespace TaskerFunction
{
	// General
	void updateControlDiode(); // built in diode is blinked once per second
	void checkCalibrations(); // Check if there is a need to calibrate one of the module and perform it if needed
	
	// Steering
	void readXY_angles();
	void readCompass();
	void stabilize();
	void updatePressureAndAltHold(); // update altHold PID if needed and do other baro stuff

	// Communication
	void updateSending();
	void updateReceiving();
}


#endif

