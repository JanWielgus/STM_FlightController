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
		FlightMode* objPtr = nullptr; // flight mode pointer
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

	static const uint8_t amtOfFlightModes = (uint8_t)FlightModeType::LAST_FLIGHT_MODE + 1;
	flightModePackageType flightModesArray[amtOfFlightModes]; // array of all available flight modes

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

