// StabilizeFlightMode.h

#ifndef _STABILIZEFLIGHTMODE_h
#define _STABILIZEFLIGHTMODE_h

#include "arduino.h"
#include <MyPID.h>
#include "FlightMode.h"
#include "Interfaces.h"


class StabilizeFlightMode: public FlightMode
{
public:
	StabilizeFlightMode(IVirtualPilot* virtualPilot);
	void run() override;
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
	void updateLevelingStuff();
	void updateHeadingStuff();

	// Help methods
	void integrateHeadingToHold(); // use roatate stick to calculate headingToHold
	void correctHeadingError(); // Correct heading to be in range of -180 : 180
};

#endif

