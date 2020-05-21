// 
// 
// 

#include "VirtualPilot.h"
#include "Storage.h"


VirtualPilot::VirtualPilot(FC_ObjectTasker* taskerPointer)
	: tasker(*taskerPointer)
{
	flightModesArraySize = 5;
	flightModesArray = new IFlightMode* [flightModesArraySize];
}


VirtualPilot::~VirtualPilot()
{
	delete[] flightModesArray;
}



void VirtualPilot::runVirtualPilot()
{
	// Run current flight mode code and get the result virtual sticks values
	currentFlightMode->run();
	virtualSticksType* curStick = currentFlightMode->getVirtualSticks();


	// when drone is disarmed motors will not spin (unarmed flight mode)
	// when disconnected from the pilot, motors will stop (check if config::DisableMotorsWhenConnectionIsLost is true)

	// there is unarmed flight mode which is activated when drone is unarmed
	// in this flight mode all sticks are always set to 0
	// during flight mode changing it arm and disarm motors as needed



	// How to use
	// 
	// VirtualPilot use current flight mode virtualSticks as pid outputs

	Storage::motors.setOnTL(curStick->throttle - curStick->TB + curStick->LR - curStick->rotate);
	Storage::motors.setOnTR(curStick->throttle - curStick->TB - curStick->LR + curStick->rotate);
	Storage::motors.setOnBR(curStick->throttle + curStick->TB - curStick->LR - curStick->rotate);
	Storage::motors.setOnBL(curStick->throttle + curStick->TB + curStick->LR + curStick->rotate);
	Storage::motors.forceMotorsExecution();
}


bool VirtualPilot::setFlightMode(FlightModeType flightModeToSet)
{
	bool result = false;

	// Find flight mode pointer in the array
	for (int i = 0; i < amtOfFlightModes; i++)
	{
		if (flightModesArray[i]->getType() == flightModeToSet)
		{
			currentFlightMode = flightModesArray[i];
			result = true;
		}
	}

	// Return false if not found flight mode with that type
	if (result == false)
		return false;

	// reset state of not related flight modes
	// and prepate for change related ones
	for (int i = 0; i < amtOfFlightModes; i++)
	{
		if (!currentFlightMode->checkIfRelated(flightModesArray[i]))
			flightModesArray[i]->reset();
		else
			flightModesArray[i]->prepare();
	}

	return true;
}


bool VirtualPilot::addFlightMode(IFlightMode* flightModeToAdd)
{
	// Check if this flight mode type is not already in the array
	FlightModeType toAddType = flightModeToAdd->getType();
	for (int i = 0; i < amtOfFlightModes; i++)
		if (flightModesArray[i]->getType() == toAddType)
			return false;

	// Make sure array is big enough
	if (flightModesArraySize < amtOfFlightModes + 1)
	{
		// Create bigger array (for two new flight modes)
		flightModesArraySize = amtOfFlightModes + 2;
		IFlightMode** newArray = new IFlightMode* [flightModesArraySize];

		// Copy previous flight modes to the new array
		for (int i = 0; i < amtOfFlightModes; i++)
			newArray[i] = flightModesArray[i];

		// Now safely delete smaller array
		delete[] flightModesArray;
		flightModesArray = newArray; // change pointer
	}

	// Add new flight mode on the end
	flightModesArray[amtOfFlightModes] = flightModeToAdd;
	amtOfFlightModes++;

	return true;
}


FlightModeType VirtualPilot::getCurrentFlightModeType()
{
	return currentFlightMode->getType();
}



