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
	if (Storage::com.connectionStability() > 1)
	{
		headingToHold += ((float)(Storage::com.received.steer.rotate / 2) * config::MainDeltaTimeInSeconds);
	}
}
