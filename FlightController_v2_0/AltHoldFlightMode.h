// AltHoldFlightMode.h

#ifndef _ALTHOLDFLIGHTMODE_h
#define _ALTHOLDFLIGHTMODE_h

#include "arduino.h"
#include <MyPID.h>
#include "FlightMode.h"
#include "StabilizeFlightMode.h"


class AltHoldFlightMode: public FlightMode
{
public:
	AltHoldFlightMode(StabilizeFlightMode* stabilizeFM);
	void execute() override;
	void reset() override; // reset only the AltHold part

public:
	float getPressureToHold();


private:
	float pressureToHold = 0;

};

#endif

