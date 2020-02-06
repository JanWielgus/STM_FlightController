// VirtualPilot.h

#ifndef _VIRTUALPILOT_h
#define _VIRTUALPILOT_h

#include "arduino.h"
#include "FlightMode.h"
#include "StabilizeFlightMode.h"
#include "AltHoldFlightMode.h"
#include "PosHoldFlightMode.h"


class VirtualPilot
{
public:
	VirtualPilot();
	~VirtualPilot();

	// public methods
	// ..


private:
	FlightMode* currentFlightMode; // pointer to the current flight mode (abstract type)

	// Particular flight modes object pointers
	StabilizeFlightMode* stabilizeFlightMode;
	AltHoldFlightMode* altHoldFlightMode;
	PosHoldFlightMode* posHoldFlightMode;
};


#endif

