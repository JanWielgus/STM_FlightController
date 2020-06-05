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

	Flight modes only update their virtual sticks (using PID controllers).
	The rest is done inside the virtual pilot.

	!!!!!
*/



class FlightMode: public IFlightMode
{
public:
	FlightMode(FlightModeType typeToSet, IFlightMode* baseFlM, IVirtualPilot* virtualPilot);
	virtual void run() override = 0;
	virtual void reset() override = 0;
	virtual void prepare() override = 0;
	virtualSticksType* getVirtualSticksPtr() override; // return static virtual sticks values as structure

	bool checkIfRelated(const IFlightMode* toCheck) override;
	FlightModeType getType() override;


	// protected components
protected:
	IFlightMode* const baseFlightMode;  // ! To initialize inside derivative class (using constructor of this class)
	static virtualSticksType virtualSticks;	// There is only onle instance of virtual sticks for all flight modes
											// Before running any flight mode, virtual pilot puts there received sticks values
											// All flight modes make calculations only based on this values
											// (just modify this values and do nothing when not affected by this flight mode)
											// Flight mode decide whether to execute its own code before its base flight mode or after

	void executeBaseFlightMode();
	void resetVirtualStickValues();		// Sets all virtual sticks values to 0


private:
	const FlightModeType type;
};


#endif

