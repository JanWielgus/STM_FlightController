// 
// 
// 

#include "PosHoldFlightMode.h"
#include "AltHoldFlightMode.h"
#include "SharedDataTypes.h"


PosHoldFlightMode::PosHoldFlightMode(IFlightMode* altHoldFM, IVirtualPilot* virtualPilot)
	: FlightMode(FlightModeType::POS_HOLD, altHoldFM, virtualPilot)
{

}


void PosHoldFlightMode::run()
{
	executeBaseFlightMode();



}


void PosHoldFlightMode::reset()
{

}


void PosHoldFlightMode::prepare()
{
}
