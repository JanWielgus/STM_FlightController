// PosHoldFlightMode.h

#ifndef _POSHOLDFLIGHTMODE_h
#define _POSHOLDFLIGHTMODE_h

#include "arduino.h"
#include <MyPID.h>
#include "FlightMode.h"
#include "StabilizeFlightMode.h"
#include "StabilizeFlightMode.h"
#include "AltHoldFlightMode.h"
#include "Interfaces.h"


class PosHoldFlightMode: public FlightMode
{
public:
	PosHoldFlightMode(IFlightMode* altHoldFM, IVirtualPilot* virtualPilot);
	void run() override;
	void reset() override; // reset only the PosHold part
};

#endif

