// 
// 
// 

#include "AltHoldFlightMode.h"
#include "StabilizeFlightMode.h"
#include "CustomDataTypes.h"


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
