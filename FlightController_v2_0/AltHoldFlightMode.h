// AltHoldFlightMode.h

#ifndef _ALTHOLDFLIGHTMODE_h
#define _ALTHOLDFLIGHTMODE_h

#include "arduino.h"
#include <MyPID.h>
#include "FlightMode.h"
#include "StabilizeFlightMode.h"
#include "Interfaces.h"


class AltHoldFlightMode: public FlightMode
{
public:
	AltHoldFlightMode(IFlightMode* stabilizeFM, IVirtualPilot* virtualPilot);
	void run() override;
	void reset() override; // reset only the AltHold part
	void prepare() override; // prepate for action

public:
	float getPressureToHold();


private:
	float pressureToHold = 0;
	int16_t altHoldBaseThrottle = 0; // should be throttle value when altitude do not change
	float pressureError;


private:
	void integratePressureToHold();
};

#endif

