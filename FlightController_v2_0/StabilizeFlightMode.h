// StabilizeFlightMode.h

#ifndef _STABILIZEFLIGHTMODE_h
#define _STABILIZEFLIGHTMODE_h

#include "arduino.h"
#include <MyPID.h>


class StabilizeFlightMode: public FlightMode
{
public:
	StabilizeFlightMode();
	void execute();
	void reset();


private:
	// PID controllers objects are created inside the Storage files
	// Use that objects. This will speed up the program and eliminate
	// multiple copies of objects and addidional pointers to that objects

	float headingError;
};

#endif

