// 
// 
// 

#include "VirtualPilot.h"
#include "Storage.h"


VirtualPilot::VirtualPilot()
{
	stabilizeFlightMode = new StabilizeFlightMode;
	altHoldFlightMode = new AltHoldFlightMode(stabilizeFlightMode);
	posHoldFlightMode = new PosHoldFlightMode(altHoldFlightMode);
}


VirtualPilot::~VirtualPilot()
{
	delete stabilizeFlightMode;
	delete altHoldFlightMode;
	delete posHoldFlightMode;
}



