// PosHoldFlightMode.h

#ifndef _POSHOLDFLIGHTMODE_h
#define _POSHOLDFLIGHTMODE_h

#include "arduino.h"
#include <MyPID.h>
#include "FlightMode.h"
#include "StabilizeFlightMode.h"
#include "StabilizeFlightMode.h"
#include "AltHoldFlightMode.h"


class PosHoldFlightMode: public FlightMode
{
public:
	PosHoldFlightMode(AltHoldFlightMode* altHoldFM);
	void execute() override;
	void reset() override; // reset only the PosHold part

};

#endif

