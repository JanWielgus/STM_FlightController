// VirtualPilot.h

#ifndef _VIRTUALPILOT_h
#define _VIRTUALPILOT_h

#include "arduino.h"
#include "config.h"
#include "FlightMode.h"
#include "StabilizeFlightMode.h"
#include "AltHoldFlightMode.h"
#include "PosHoldFlightMode.h"
#include "Storage.h"


class VirtualPilot
{
private:
	struct flightModePackageType
	{
		FlightMode* objPtr; // flight mode pointer
		uint8_t layer; // layer of the flight mode
	};


public:
	VirtualPilot();
	~VirtualPilot();

	// public methods
	void runVirtualPilot();
	void setFlightMode(FlightModeType flightModeToSet);
	FlightModeType getCurrentFlightMode();
	


private:
	flightModePackageType* currentFlightMode; // pointer to the current flight mode (abstract type)

	flightModePackageType* flightModesArray; // array of all available flight modes
	uint8_t amtOfFlightModes = 0;

	// Used flight modes
	StabilizeFlightMode stabilizeFlightMode = StabilizeFlightMode();
	AltHoldFlightMode altHoldFlightMode = AltHoldFlightMode(&stabilizeFlightMode);
	PosHoldFlightMode posHoldFlightMode = PosHoldFlightMode(&altHoldFlightMode);
	// other flight modes ...


private:
	bool fillFlightModesArrayRow(uint8_t layer, FlightMode* ptr);
	FlightMode* getFlightModePtrByType(FlightModeType flightModeType);
};


#endif

