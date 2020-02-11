// FlightMode.h

#ifndef _FLIGHTMODE_h
#define _FLIGHTMODE_h

#include "arduino.h"
#include "SharedDataTypes.h"


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

