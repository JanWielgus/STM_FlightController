// 
// 
// 

#include "AltHoldFlightMode.h"
#include "StabilizeFlightMode.h"
#include "SharedDataTypes.h"


AltHoldFlightMode::AltHoldFlightMode(StabilizeFlightMode* stabilizeFM)
	: FlightMode(FlightModeType::ALT_HOLD)
{
	this->baseFlightMode = stabilizeFM;

	//...
}


void AltHoldFlightMode::execute()
{
	this->baseFlightMode->execute();



}


void AltHoldFlightMode::reset()
{

}



float AltHoldFlightMode::getPressureToHold()
{
	return pressureToHold;
}
