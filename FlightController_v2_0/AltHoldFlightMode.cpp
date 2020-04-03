// 
// 
// 

#include "AltHoldFlightMode.h"
#include "StabilizeFlightMode.h"
#include "SharedDataTypes.h"


AltHoldFlightMode::AltHoldFlightMode(IFlightMode* stabilizeFM, IVirtualPilot* virtualPilot)
	: FlightMode(FlightModeType::ALT_HOLD, stabilizeFM, virtualPilot)
{
}


void AltHoldFlightMode::run()
{
	this->baseFlightMode->run();



}


void AltHoldFlightMode::reset()
{

}



float AltHoldFlightMode::getPressureToHold()
{
	return pressureToHold;
}
