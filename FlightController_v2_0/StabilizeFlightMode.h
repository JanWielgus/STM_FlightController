// StabilizeFlightMode.h

#ifndef _STABILIZEFLIGHTMODE_h
#define _STABILIZEFLIGHTMODE_h

#include "arduino.h"
#include <MyPID.h>
#include "FlightMode.h"


class StabilizeFlightMode: public FlightMode
{
public:
	StabilizeFlightMode();
	void execute() override;
	void reset() override;

public:
	float getHeadingToHold();


private:
	// PID controllers objects are created inside the Storage files
	// Use that objects. This will speed up the program and eliminate
	// multiple copies of objects and addidional pointers to that objects

	float headingToHold = 0; // calculated value based on the pilot rotate stick to hold by the drone
	float headingError;


private:
	void updateHeadingToHold();
};

#endif

