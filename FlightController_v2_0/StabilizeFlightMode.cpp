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
	updateHeadingToHold();



}


void StabilizeFlightMode::reset()
{

}



float StabilizeFlightMode::getHeadingToHold()
{
	return headingToHold;
}


void StabilizeFlightMode::updateHeadingToHold()
{
	// Integrate yaw stick value only if connection is stable
	if (com.connectionStability() > 1)
	{
		headingToHold += ((float)(com.received.steer.rotate / 2) * config::MainDeltaTimeInSeconds);
	}
}
