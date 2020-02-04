// AltHoldFlightMode.h

#ifndef _ALTHOLDFLIGHTMODE_h
#define _ALTHOLDFLIGHTMODE_h

#include "arduino.h"


class AltHoldFlightMode: public FlightMode
{
public:
	void execute();
	void reset();
};

#endif

