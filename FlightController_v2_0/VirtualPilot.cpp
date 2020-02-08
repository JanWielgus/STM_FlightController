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
	fillFlightModesArrayRow(0, &stabilizeFlightMode, FlightModeType::STABILIZE);
	fillFlightModesArrayRow(1, &altHoldFlightMode, FlightModeType::ALT_HOLD);
	fillFlightModesArrayRow(2, &posHoldFlightMode, FlightModeType::POS_HOLD);
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

}


FlightModeType VirtualPilot::getCurrentFlightMode()
{
	return currentFlightMode->type;
}


bool VirtualPilot::fillFlightModesArrayRow(uint8_t layer, FlightMode* ptr, FlightModeType type)
{
	// 'type' is the index in the array 

	if ((uint8_t)type >= amtOfFlightModes)
		return false;

	flightModesArray[type].layer = layer;
	flightModesArray[type].objPtr = ptr;
	flightModesArray[type].type = type;
	
	return true;
}


FlightMode* VirtualPilot::getFlightModePtrByType(FlightModeType flightModeType)
{
	for (int i = 0; i < amtOfFlightModes; i++)
	{
		if (flightModeType == this->flightModesArray[i].type)
			return this->flightModesArray[i].objPtr;
	}
}



