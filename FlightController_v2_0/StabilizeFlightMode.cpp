// 
// 
// 

#include "StabilizeFlightMode.h"
#include "Storage.h"
#include "SharedDataTypes.h"


StabilizeFlightMode::StabilizeFlightMode()
	: FlightMode(FlightModeType::STABILIZE)
{

}


void StabilizeFlightMode::execute()
{
	updateLevelingStuff();
	updateHeadingStuff();



}


void StabilizeFlightMode::reset()
{
	// Reset virtual sticks
	// Reset leveling controllers
	// Reset yaw controller
	// Update headingToHold to the current heading
	// 
}



float StabilizeFlightMode::getHeadingToHold()
{
	return headingToHold;
}


void StabilizeFlightMode::updateLevelingStuff()
{
}


void StabilizeFlightMode::updateHeadingStuff()
{
	// Integrate yaw (rotate) stick value
	integrateHeadingToHold();

	// Calculate the error
	headingError = headingToHold - reading.heading;
	correctHeadingError(); // Correct error to be in range of [-180, 180]

	// Calculate the pid value
	virtualSticks.rotate = yawPID.updateController(headingError);

	

	// Think about when to reset the PID controller
	// Maybe crate a new flag or maybe not and there is a better solution
	// Problem is that when in altHold the actual throttle can be 0 and drone have to fly
	// But stabilize dont have to access to the virtual stick after flight mode because it is run before alt hold
}



void StabilizeFlightMode::integrateHeadingToHold()
{
	// Integrate only if connection is stable
	if (Storage::com.connectionStability() > 1)
		headingToHold += ((float)(Storage::com.received.steer.rotate / 2) * config::MainDeltaTimeInSeconds);
}


void StabilizeFlightMode::correctHeadingError()
{
	if (headingError > 180)
		headingError -= 360;
	else if (headingError < -180)
		headingError += 360;
}
