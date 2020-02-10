// 
// 
// 

#include "FlightMode.h"


bool FlightMode::checkIfFromThisBranch(FlightMode* toCheck)
{
	// Sprawdz czy ten tryb lotu to jest ten albo wywolaj dla bazowego trybu lotu sprawdzenie
	// Sprawdz czy tryb bazowy nie jest nullptr (moze to byc pierwszy tryb w hierarchii)
	// Ma to byc uzyte w ustawianiu trybu lotu zeby resetowac wszystkie dziedziczace tryby lotu 
	// oraz tryby lotu z innych galezi dziedziczenia

	if (toCheck == this) // this is the searched class
		return true;

	else if (baseFlightMode != nullptr)
		return baseFlightMode->checkIfFromThisBranch(toCheck);

	else
		return false;
}


void FlightMode::executeBaseFlightMode()
{
	if (baseFlightMode != nullptr)
		baseFlightMode->execute();
}
