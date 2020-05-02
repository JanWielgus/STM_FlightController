// UnarmedFlightMode.h

#ifndef _UNARMEDFLIGHTMODE_h
#define _UNARMEDFLIGHTMODE_h

#include "arduino.h"
#include "FlightMode.h"
#include "Interfaces.h"

/*
	This flight mode is used when drone is unarmed.
	Everything that happens when drone is unarmed is just here.

	No need to reset PID controllers here. This is done in proper flight modes
	because they are not set (VirtualPilot execute their reset() methods)
*/


class UnarmedFlightMode : public FlightMode
{
public:
	UnarmedFlightMode(IVirtualPilot* virtualPilot);
	void run() override;
	void reset() override;
	void prepare() override;
};


#endif

