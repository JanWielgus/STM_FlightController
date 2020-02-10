// FlightMode.h

#ifndef _FLIGHTMODE_h
#define _FLIGHTMODE_h

#include "arduino.h"


class FlightMode
{
public:
	virtual void execute() = 0;
	virtual void reset() = 0;

	// Ta funkcja ma sprawdzac czy jakis tryb lotu jest z tej galezi
	bool checkIfFromThisBranch(FlightMode* toCheck);


	// protected components
protected:
	FlightMode* baseFlightMode = nullptr;

	void executeBaseFlightMode();


	// Pomysl czy kazdy tryb lotu nie powinien zawierac swojego typu
	// (zmiennej typu enumerator typTrybuLotu)
};


#endif

