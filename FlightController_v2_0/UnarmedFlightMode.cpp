// 
// 
// 

#include "UnarmedFlightMode.h"


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
	motors.setMotorState(false);

	// turn off blue diode
	digitalWrite(config::pin.blueDiode, LOW);
}


// executed once, when exit from unarmed flight mode
void UnarmedFlightMode::reset()
{
	// arm motors
	motors.setMotorState(true);

	// turn on blue diode
	digitalWrite(config::pin.blueDiode, HIGH);
}
