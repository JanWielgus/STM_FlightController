// 
// 
// 

#include "FlightMode.h"


FlightMode::FlightMode(FlightModeType typeToSet, IFlightMode* baseFlM, IVirtualPilot* virtualPilot)
	: type(typeToSet), baseFlightMode(baseFlM)
{
	// Add this object to the virtual pilot
	virtualPilot->addFlightMode(this);
}


bool FlightMode::checkIfRelated(const IFlightMode* toCheck)
{
	// Sprawdz czy ten tryb lotu to jest ten albo wywolaj dla bazowego trybu lotu sprawdzenie
	// Sprawdz czy tryb bazowy nie jest nullptr (moze to byc pierwszy tryb w hierarchii)
	// Ma to byc uzyte w ustawianiu trybu lotu zeby resetowac wszystkie dziedziczace tryby lotu 
	// oraz tryby lotu z innych galezi dziedziczenia

	if (toCheck == this) // this is the searched class
		return true;
	else if (baseFlightMode != nullptr)
		return baseFlightMode->checkIfRelated(toCheck);
	else
		return false;
}


FlightModeType FlightMode::getType()
{
	return this->type;
}


virtualSticksType* FlightMode::getVirtualSticks()
{
	return &this->virtualSticks;
}


void FlightMode::executeBaseFlightMode()
{
	if (baseFlightMode != nullptr)
		baseFlightMode->run();
}

