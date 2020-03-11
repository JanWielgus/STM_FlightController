// 
// 
// 

#include "VirtualPilot.h"


VirtualPilot::VirtualPilot(FC_ObjectTasker* taskerPointer, FlightMode* defaultFlightMode)
	: tasker(*taskerPointer)
{
	// Set each pointer to nullptr
	for (int i = 0; i < AmtOfFlightModes; i++)
		flightModesArray[i] = nullptr;

	addFlightMode(defaultFlightMode);
	setFlightMode(defaultFlightMode->getType()); // default flight mode
}


VirtualPilot::~VirtualPilot()
{
}



void VirtualPilot::runVirtualPilot()
{

}


bool VirtualPilot::setFlightMode(FlightModeType flightModeToSet)
{
	FlightMode* toSet = getFlightModePtrByType(flightModeToSet);
	if (isNotNullptr(toSet))
	{
		currentFlightMode = toSet;

		// Reset other flight modes (not involved in the current flight mode)
		for (int i = 0; i < AmtOfFlightModes; i++)
			// If pointer is not null AND checked flight mode is not from current flight mode branch
			if (isNotNullptr(flightModesArray[i]) &&
				!currentFlightMode->checkIfFromThisBranch(flightModesArray[i]))
				flightModesArray[i]->reset();

		return true;
	}

	return false;
}


// This method adds flight mode to it's fixed position in array
void VirtualPilot::addFlightMode(FlightMode* flightModeToAdd)
{
	// 'type' is the index in the array
	uint8_t type = (uint8_t)flightModeToAdd->getType();
	bool wasEmpty = flightModesArray[type] == nullptr ? true : false; // checks if this array row was empty
	flightModesArray[type] = flightModeToAdd;

	// FlightMode inherits from FC_Task so it can be added to the tasker tasks array
	// Add to the tasker only if it was empty before
	if (wasEmpty)
		tasker.addTask(flightModeToAdd, config::MainInterval, 0);
}


FlightModeType VirtualPilot::getCurrentFlightMode()
{
	return currentFlightMode->getType();
}


FlightMode* VirtualPilot::getFlightModePtrByType(FlightModeType flightModeType)
{
	for (int i = 0; i < AmtOfFlightModes; i++)
	{
		if (isNotNullptr(this->flightModesArray[i]) &&
			this->flightModesArray[i]->getType() == flightModeType)
			return this->flightModesArray[i];
	}

	return nullptr;
}


bool VirtualPilot::isNotNullptr(FlightMode* flightMode)
{
	return flightMode != nullptr;
}



