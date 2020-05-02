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
	executeBaseFlightMode();



}


void AltHoldFlightMode::reset()
{

}


void AltHoldFlightMode::prepare()
{
}



float AltHoldFlightMode::getPressureToHold()
{
	return pressureToHold;
}
