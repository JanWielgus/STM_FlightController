// Failsafe.h
/*
	Handling all fail scenarios.
	Added to tasker as task (20Hz should be fine)
*/

#ifndef _FAILSAFE_h
#define _FAILSAFE_h

#include "arduino.h"
#include <FC_Task.h>


class Failsafe : public FC_Task
{
	void execute() override;

	// failsafe actions
	void cutoffMotorsIfExceededAngle();
	void suppressMotorsAndDisarmWhenLostSignal(); // in 0.5 second gradually suppress motors and in the end disarm
};

#endif

