// FlightMode.h

#ifndef _FLIGHTMODE_h
#define _FLIGHTMODE_h

#include "arduino.h"
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



class FlightMode
{
public:
	FlightMode(FlightModeType typeToSet);
	virtual void execute() = 0;
	virtual void reset() = 0;

	bool checkIfFromThisBranch(FlightMode* toCheck);
	FlightModeType getType();


	// protected components
protected:
	FlightMode* baseFlightMode = nullptr;
	const FlightModeType type;

	void executeBaseFlightMode();


	// Pomysl czy kazdy tryb lotu nie powinien zawierac swojego typu
	// (zmiennej typu enumerator typTrybuLotu)
};


#endif

