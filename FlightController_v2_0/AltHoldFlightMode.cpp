// 
// 
// 

#include "AltHoldFlightMode.h"
#include "Storage.h"


AltHoldFlightMode::AltHoldFlightMode(StabilizeFlightMode* stabilizeFM)
	: FlightMode(FlightModeType::ALT_HOLD)
{
	this->baseFlightMode = stabilizeFM;

	//...
}


void AltHoldFlightMode::execute()
{

}


void AltHoldFlightMode::reset()
{

}
