// 
// 
// 

#include "AltHoldFlightMode.h"
#include "StabilizeFlightMode.h"
#include "SharedDataTypes.h"
#include "Storage.h"
#include "config.h"


AltHoldFlightMode::AltHoldFlightMode(IFlightMode* stabilizeFM, IVirtualPilot* virtualPilot)
	: FlightMode(FlightModeType::ALT_HOLD, stabilizeFM, virtualPilot)
{
}


void AltHoldFlightMode::run()
{
	executeBaseFlightMode();



}


// called when exit from this flight mode
void AltHoldFlightMode::reset()
{
	// Reset virtual sticks
	resetVirtualStickValues();

	// Reset pid controller
	Storage::altHoldPID.resetController();
}


// called when entering to this flight mode
// can be called several times during working
void AltHoldFlightMode::prepare()
{
}



float AltHoldFlightMode::getPressureToHold()
{
	return pressureToHold;
}
