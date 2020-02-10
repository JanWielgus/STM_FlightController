// 
// 
// 

#include "VirtualPilot.h"
#include "Storage.h"


VirtualPilot::VirtualPilot()
{
	// Fill flight modes array  ( id, layer, pointer )
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
	// Find and set the current flight mode
	for (int i = 0; i < amtOfFlightModes; i++)
		if (flightModesArray[i].objPtr->getType() == flightModeToSet)
		{
			currentFlightMode = &flightModesArray[i];
			break;
		}


	// Reset other flight modes (not involved in the current flight mode)
	for (int i = 0; i < amtOfFlightModes; i++)
		// If pointer is not null AND checked flight mode is not from current flight mode branch
		if (flightModesArray[i].objPtr != nullptr &&
			!currentFlightMode->objPtr->checkIfFromThisBranch(flightModesArray[i].objPtr))
			flightModesArray[i].objPtr->reset();
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



