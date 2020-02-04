// StabilizeFlightMode.h

#ifndef _STABILIZEFLIGHTMODE_h
#define _STABILIZEFLIGHTMODE_h

#include "arduino.h"
#include <MyPID.h>


class StabilizeFlightMode: public FlightMode
{
public:
	StabilizeFlightMode(MyPID* levelX_controller, MyPID* levelY_controller);
	void execute();
	void reset();
};

#endif

