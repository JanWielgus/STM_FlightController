// 
// 
// 

#include "PosHoldFlightMode.h"
#include "AltHoldFlightMode.h"
#include "SharedDataTypes.h"


PosHoldFlightMode::PosHoldFlightMode(AltHoldFlightMode* altHoldFM)
	: FlightMode(FlightModeType::POS_HOLD)
{
	this->baseFlightMode = altHoldFM;

	//...
}


void PosHoldFlightMode::execute()
{
	this->baseFlightMode->execute();



}


void PosHoldFlightMode::reset()
{

}
