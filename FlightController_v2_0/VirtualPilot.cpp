// 
// 
// 

#include "VirtualPilot.h"
#include "Storage.h"


VirtualPilot::VirtualPilot()
{
	// Set each pointer to nullptr
	for (int i = 0; i < amtOfFlightModes; i++)
		flightModesArray[i] = nullptr;

	setFlightMode(FlightModeType::STABILIZE); // default flight mode
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
		if (flightModesArray[i]->getType() == flightModeToSet)
		{
			currentFlightMode = flightModesArray[i];
			break;
		}


	// Reset other flight modes (not involved in the current flight mode)
	for (int i = 0; i < amtOfFlightModes; i++)
		// If pointer is not null AND checked flight mode is not from current flight mode branch
		if (flightModesArray[i] != nullptr &&
			!currentFlightMode->checkIfFromThisBranch(flightModesArray[i]))
			flightModesArray[i]->reset();
}


void VirtualPilot::addFlightMode(FlightMode* flightModeToAdd)
{
	// 'type' is the index in the array
	uint8_t type = (uint8_t)flightModeToAdd->getType();

	flightModesArray[type] = flightModeToAdd;
}


FlightModeType VirtualPilot::getCurrentFlightMode()
{
	return currentFlightMode->getType();
}


FlightMode* VirtualPilot::getFlightModePtrByType(FlightModeType flightModeType)
{
	for (int i = 0; i < amtOfFlightModes; i++)
	{
		if (flightModeType == this->flightModesArray[i]->getType())
			return this->flightModesArray[i];
	}
}



