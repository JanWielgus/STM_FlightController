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

	// Calculate the pilot change in altitude
	integratePressureToHold();

	// Calculate the error and final throttle value
	pressureError = Storage::reading.pressure - pressureToHold;
	pressureError *= 5;
	int16_t tempThrottle = Storage::altHoldPID.updateController(pressureError);
	tempThrottle += altHoldBaseThrottle;
	virtualSticks.throttle = constrain(tempThrottle,
										config::flightModeConfig.altHoldMinThrottle,
										config::flightModeConfig.altHoldMaxThrottle);
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
	// For now it is const value set in config
	if (altHoldBaseThrottle == 0) // if not yet set
		//altHoldBaseThrottle = virtualSticks.throttle;
		this->altHoldBaseThrottle = config::flightModeConfig.altHoldBaseThrottle;


	// Set initial pressure to hold
	pressureToHold = Storage::reading.pressure;
}



float AltHoldFlightMode::getPressureToHold()
{
	return pressureToHold;
}


void AltHoldFlightMode::integratePressureToHold()
{
	// Do not integrate if connection is not stable
	if (Storage::comm.getConnectionStability() < 20)
		return;


	float toIntegrate = (float)(virtualSticks.throttle - 500);
	// if value throttle is moved from center (500) by more than offset
	if (abs(toIntegrate) > config::flightModeConfig.altHoldThrottleCenterOffset)
	{
		// Make the value to start from 0
		if (toIntegrate > 0)
			toIntegrate -= config::flightModeConfig.altHoldThrottleCenterOffset;
		else
			toIntegrate += config::flightModeConfig.altHoldThrottleCenterOffset;

		// Divide the value to slower down changes
		// 6 pressure value change is about 0.5 meter
		toIntegrate /= 50.f;

		// integrate
		pressureToHold -= toIntegrate * config::MainDeltaTimeInSeconds;
	}
}
