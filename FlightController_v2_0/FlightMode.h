// FlightMode.h

#ifndef _FLIGHTMODE_h
#define _FLIGHTMODE_h

#include "arduino.h"


class FlightMode
{
public:
	virtual void execute() = 0;
	virtual void reset() = 0;


	// private components
private:
};


#endif

