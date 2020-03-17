// 
// 
// 

#include "VirtualPilot.h"


VirtualPilot::VirtualPilot(FC_ObjectTasker* taskerPointer)
	: tasker(*taskerPointer)
{
	// Set each pointer to nullptr
	for (int i = 0; i < AmtOfFlightModes; i++)
		flightModesArray[i] = nullptr;
}


VirtualPilot::~VirtualPilot()
{
}



void VirtualPilot::runVirtualPilot()
{
	currentFlightMode->execute();
	virtualSticksType* curStick = currentFlightMode->getVirtualSticks();

	// when drone is disarmed motors will not spin
	// when disconnected from the pilot, motors will stop (check if config::DisableMotorsWhenConnectionIsLost is true)



	// !!!
	// How to use
	// 
	// VirtualPilot use current flight mode virtualSticks as pid outputs
	// If needed create in config the scale values to multiply by


	motors.setOnTL(curStick->throttle + curStick->TB + curStick->LR - curStick->rotate); // BR
	motors.setOnTL(curStick->throttle + curStick->TB - curStick->LR + curStick->rotate); // BL
	motors.setOnTL(curStick->throttle - curStick->TB - curStick->LR - curStick->rotate); // TL
	motors.setOnTL(curStick->throttle - curStick->TB + curStick->LR + curStick->rotate); // TR
}


void VirtualPilot::execute()
{
	runVirtualPilot();
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
bool VirtualPilot::addFlightMode(FlightMode* flightModeToAdd, bool isDefault)
{
	// 'type' is the index in the array
	uint8_t type = (uint8_t)flightModeToAdd->getType();

	// if this flight mode is null (was not added before)
	if (flightModesArray[type] == nullptr)
	{
		flightModesArray[type] = flightModeToAdd;

		// check if this flight mode is default one
		if (isDefault)
			setFlightMode(flightModeToAdd->getType());

		return true;
	}

	return false;
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



