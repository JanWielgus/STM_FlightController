// FlightMode.h

#ifndef _FLIGHTMODE_h
#define _FLIGHTMODE_h

#include "arduino.h"
#include "SharedDataTypes.h"
#include <FC_Task.h>


class FlightMode : public FC_Task // something like implementing FC_Task interface
								  // this will allow to add flight mode to the tasker
{
public:
	FlightMode(FlightModeType typeToSet);
	virtual void execute() override = 0;
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

