// FlightMode.h

#ifndef _FLIGHTMODE_h
#define _FLIGHTMODE_h

#include "arduino.h"
#include "Interfaces.h"
#include "SharedDataTypes.h"
#include <FC_Task.h>


/*
	!!!!!

	All flight modes are running at 250Hz (if they are active ones)
	Make it to work well at that speed
	For example use extrapolations or interpolations

	Flight modes only update virtual sticks (using PID controllers).
	The rest is done inside the virtual pilot.

	!!!!!
*/



class FlightMode: public IFlightMode
{
public:
	FlightMode(FlightModeType typeToSet, IFlightMode* baseFlM, IVirtualPiltot* virtualPilot);
	virtual void reset() override = 0;
	virtualSticksType* getVirtualSticks() override; // return virtual sticks values as structure

	bool checkIfRelated(const IFlightMode* toCheck) override;
	FlightModeType getType() override;


	// protected components
protected:
	IFlightMode* const baseFlightMode; // ! To initialize inside derivative class
	const FlightModeType type;
	virtualSticksType virtualSticks;	// Eack flight mode has own virtual stick values
										// Each next fligh mode decide if want to override previous sticks value
										// VirtualPilot just use values from the last flight mode

	void executeBaseFlightMode();
};


#endif

