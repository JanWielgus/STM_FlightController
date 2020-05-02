// 
// 
// 

#include "UnarmedFlightMode.h"
#include "Storage.h"
#include "config.h"


UnarmedFlightMode::UnarmedFlightMode(IVirtualPilot* virtualPilot)
	: FlightMode(FlightModeType::UNARMED, nullptr, virtualPilot)
{
	virtualSticks.throttle = 0;
	virtualSticks.rotate = 0;
	virtualSticks.TB = 0;
	virtualSticks.LR = 0;
}


// everything that happens when drone is disarmed (excluding PID controllers reset)
void UnarmedFlightMode::run()
{
	// disarm motors
	Storage::motors.setMotorState(false);
}


// executed once, when exit from unarmed flight mode
void UnarmedFlightMode::reset()
{
	// arm motors
	Storage::motors.setMotorState(true);

	// turn on blue diode
	digitalWrite(config::pin.blueDiode, HIGH);
}


// Executed when entering to unarmed flight mode
void UnarmedFlightMode::prepare()
{
	// turn off blue diode
	digitalWrite(config::pin.blueDiode, LOW);
}
