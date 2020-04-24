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
	virtualSticks.throttle = ReceiveData::throttle;

	// rot, TB and LR are set by PID controllers
	updateLevelingStuff();
	updateHeadingStuff();



}


void StabilizeFlightMode::reset()
{
	// Reset virtual sticks
	virtualSticks.throttle = 0;
	virtualSticks.rotate = 0;
	virtualSticks.TB = 0;
	virtualSticks.LR = 0;

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
	headingToHold = Storage::reading.heading;
}



float StabilizeFlightMode::getHeadingToHold()
{
	return headingToHold;
}


void StabilizeFlightMode::updateLevelingStuff()
{
	// Update virtual sticks values using PID controllers
	virtualSticks.TB = Storage::levelXpid.updateController(Storage::reading.angle.x + (ReceiveData::TB_stick / 10.f)) + 0.5f;
	virtualSticks.LR = Storage::levelYpid.updateController(Storage::reading.angle.y - (ReceiveData::LR_stick / 10.f)) + 0.5f;

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
		headingToHold += ((float)(ReceiveData::rot_stick / 2.f) * config::MainDeltaTimeInSeconds);
}


void StabilizeFlightMode::correctHeadingError()
{
	if (headingError > 180)
		headingError -= 360;
	else if (headingError < -180)
		headingError += 360;
}
