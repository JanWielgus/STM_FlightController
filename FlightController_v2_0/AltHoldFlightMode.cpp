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
	

	// TODO: make altitude holding
	// ...
}


// called when exit from this flight mode
void AltHoldFlightMode::reset()
{
	// Reset virtual sticks
	resetVirtualStickValues();

	// Reset pid controller
	Storage::altHoldPID.resetController();
	
	// reset the altHold base throttle
	altHoldBaseThrottle = 0;
}


// called when entering to this flight mode
// can be called several times during working
void AltHoldFlightMode::prepare()
{
	// TODO: Try to figure out better way of calculating base throttle for alt hold
	// For now it is just the last throttle value given by the pilot
	if (altHoldBaseThrottle == 0) // if not yet set
		altHoldBaseThrottle = baseFlightMode->getVirtualSticks()->throttle;
}



float AltHoldFlightMode::getPressureToHold()
{
	return pressureToHold;
}


void AltHoldFlightMode::integratePressureToHold()
{
	// Integrate only if connection is stable
	if (Storage::comm.getConnectionStability() > 20)
	{
		float toIntegrate = (float)(baseFlightMode->getVirtualSticks()->throttle - altHoldBaseThrottle) / 50.f; // 6 pressure value change is about 0.5 meter
		pressureToHold -= toIntegrate * config::MainDeltaTimeInSeconds;
	}
}
