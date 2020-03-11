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
#include <FC_ObjectTasker.h>
#include <FC_Task.h>


class VirtualPilot : public FC_Task
{
public:
	VirtualPilot(FC_ObjectTasker* taskerPointer);
	~VirtualPilot();

	// public methods
	void runVirtualPilot();
	bool setFlightMode(FlightModeType flightModeToSet); // return true if setting new flight mode was successful
	bool addFlightMode(FlightMode* flightModeToAdd, bool isDefault = false); // return false if there already was flight mode with the same type added
	FlightModeType getCurrentFlightMode();
	


private:
	FlightMode* currentFlightMode; // pointer to the current flight mode (abstract type)

	static const uint8_t AmtOfFlightModes = (uint8_t)FlightModeType::LAST_FLIGHT_MODE + 1;
	FlightMode* flightModesArray[AmtOfFlightModes]; // array of all available flight modes pointers

	FC_ObjectTasker& tasker; // reference to the global tasker


private:
	void execute() override;
	FlightMode* getFlightModePtrByType(FlightModeType flightModeType);
	bool isNotNullptr(FlightMode* flightMode);
};


#endif

