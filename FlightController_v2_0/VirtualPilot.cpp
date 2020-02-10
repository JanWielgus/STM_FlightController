// 
// 
// 

#include "VirtualPilot.h"
#include "Storage.h"


VirtualPilot::VirtualPilot()
{
	// Create an array for all flight modes used by the VirtualPilot
	amtOfFlightModes = 3;
	flightModesArray = new flightModePackageType[amtOfFlightModes];

	// Fill that array     ( id, layer, pointer, type )
	fillFlightModesArrayRow(0, &stabilizeFlightMode);
	fillFlightModesArrayRow(1, &altHoldFlightMode);
	fillFlightModesArrayRow(2, &posHoldFlightMode);
	// NEXT FLIGHT MODES GOES HERE (remember to increase amtOfFlightModes!)


	setFlightMode(FlightModeType::STABILIZE);
}


VirtualPilot::~VirtualPilot()
{
}



void VirtualPilot::runVirtualPilot()
{

}


void VirtualPilot::setFlightMode(FlightModeType flightModeToSet)
{
	// Uzyj metody checkIfFromThisBranch w celu sprawdzenia czy
	// obecnie przegladana klasa jest z tej galezi trybow lotu
	// (inaczej czy jest powiazana z klasa ktora chcemy ustawic)
	// jesli nie jest to mozna resetowac jej stan





}


FlightModeType VirtualPilot::getCurrentFlightMode()
{
	return currentFlightMode->objPtr->getType();
}


bool VirtualPilot::fillFlightModesArrayRow(uint8_t layer, FlightMode* ptr)
{
	// 'type' is the index in the array
	uint8_t type = (uint8_t)ptr->getType();

	if (type >= amtOfFlightModes)
		return false;

	flightModesArray[type].layer = layer;
	flightModesArray[type].objPtr = ptr;
	
	return true;
}


FlightMode* VirtualPilot::getFlightModePtrByType(FlightModeType flightModeType)
{
	for (int i = 0; i < amtOfFlightModes; i++)
	{
		if (flightModeType == this->flightModesArray[i].objPtr->getType())
			return this->flightModesArray[i].objPtr;
	}
}



