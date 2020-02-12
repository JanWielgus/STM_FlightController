// VirtualPilot.h

#ifndef _VIRTUALPILOT_h
#define _VIRTUALPILOT_h

#include "arduino.h"
#include "config.h"
#include "FlightMode.h"
#include "StabilizeFlightMode.h"
#include "AltHoldFlightMode.h"
#include "PosHoldFlightMode.h"
#include "SharedDataTypes.h"
#include <FC_Tasker.h>


class VirtualPilot
{
public:
	VirtualPilot(FC_SimpleTasker* taskerPointer);
	~VirtualPilot();

	// public methods
	void runVirtualPilot();
	void setFlightMode(FlightModeType flightModeToSet);
	void addFlightMode(FlightMode* flightModeToAdd);
	FlightModeType getCurrentFlightMode();
	


private:
	FlightMode* currentFlightMode; // pointer to the current flight mode (abstract type)

	static const uint8_t amtOfFlightModes = (uint8_t)FlightModeType::LAST_FLIGHT_MODE + 1;
	FlightMode* flightModesArray[amtOfFlightModes]; // array of all available flight modes pointers

	FC_SimpleTasker& tasker; // reference to the global tasker


private:
	FlightMode* getFlightModePtrByType(FlightModeType flightModeType);
};


#endif

