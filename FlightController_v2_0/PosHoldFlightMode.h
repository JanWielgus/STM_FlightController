// PosHoldFlightMode.h

#ifndef _POSHOLDFLIGHTMODE_h
#define _POSHOLDFLIGHTMODE_h

#include "arduino.h"


class PosHoldFlightMode: public FlightMode
{
public:
	void execute();
	void reset();
};

#endif

