// 
// 
// 

#include "StabilizeFlightMode.h"
#include "Storage.h"
#include "SharedDataTypes.h"
#include "config.h"


StabilizeFlightMode::StabilizeFlightMode(IVirtualPilot* virtualPilot)
	: FlightMode(FlightModeType::STABILIZE, nullptr, virtualPilot)
{

}


void StabilizeFlightMode::run()
{
	// Only throttle remain unchanged
	virtualSticks.throttle = Storage::sticksFiltered.throttle;

	// rot, TB and LR are set by PID controllers
	if (Storage::sticksFiltered.throttle > config::ZeroActionThrottle) // if throttle is high enough
	{
		updateLevelingStuff();
		updateHeadingStuff();
	}
	else // if less than zero action throttle
	{
		resetVirtualStickValues();
		setHeadingToHoldToCurrentReading();
	}
}


void StabilizeFlightMode::reset()
{
	// Reset virtual sticks
	resetVirtualStickValues();

	// Reset leveling controllers
	Storage::levelXpid.resetController();
	Storage::levelYpid.resetController();

	// Reset yaw controller
	Storage::yawPID.resetController();
}


// Executed by virtualPilot after setting this flight mode
void StabilizeFlightMode::prepare()
{
	// Update headingToHold to the current heading
	setHeadingToHoldToCurrentReading();
}



float StabilizeFlightMode::getHeadingToHold()
{
	return headingToHold;
}


void StabilizeFlightMode::updateLevelingStuff()
{
	// Update virtual sticks values using PID controllers
	virtualSticks.TB = Storage::levelXpid.updateController(Storage::reading.angle.x + (Storage::sticksFiltered.TB / 10.f)) + 0.5f;
	virtualSticks.LR = Storage::levelYpid.updateController(Storage::reading.angle.y - (Storage::sticksFiltered.LR / 10.f)) + 0.5f;

	// Keep values in borders
	virtualSticks.TB = constrain(virtualSticks.TB, -500, 500);
	virtualSticks.LR = constrain(virtualSticks.LR, -500, 500);
}


void StabilizeFlightMode::updateHeadingStuff()
{
	// Integrate yaw (rotate) stick value
	integrateHeadingToHold();

	// Calculate the error
	headingError = headingToHold - Storage::reading.heading;
	correctHeadingError(); // Correct error to be in range of [-180, 180]

	// Calculate the pid value
	virtualSticks.rotate = Storage::yawPID.updateController(headingError);

	// Constrain
	virtualSticks.rotate = constrain(virtualSticks.rotate, -500, 500);
}



void StabilizeFlightMode::integrateHeadingToHold()
{
	// Integrate only if connection is stable
	if (Storage::comm.getConnectionStability() > 20)
		headingToHold += ((float)(Storage::sticksFiltered.rotate / 2.f) * config::MainDeltaTimeInSeconds);
}


void StabilizeFlightMode::correctHeadingError()
{
	if (headingError > 180)
		headingError -= 360;
	else if (headingError < -180)
		headingError += 360;
}


void StabilizeFlightMode::setHeadingToHoldToCurrentReading()
{
	headingToHold = Storage::reading.heading;
}
