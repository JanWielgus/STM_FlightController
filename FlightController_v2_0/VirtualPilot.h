// VirtualPilot.h

#ifndef _VIRTUALPILOT_h
#define _VIRTUALPILOT_h

#include "arduino.h"
#include "Interfaces.h"
#include "SharedDataTypes.h"
#include <FC_ObjectTasker.h>
#include <FC_Task.h>


class VirtualPilot : public IVirtualPiltot, public FC_Task
{
public:
	VirtualPilot(FC_ObjectTasker* taskerPointer);
	~VirtualPilot();

	// public methods
	void runVirtualPilot() override;
	bool setFlightMode(FlightModeType flightModeToSet) override; // return true if setting new flight mode was successful
	bool addFlightMode(IFlightMode* flightModeToAdd) override; // return false if there already was flight mode with the same type added
	FlightModeType getCurrentFlightModeType() override;
	


private:
	IFlightMode* currentFlightMode; // pointer to the current flight mode (abstract type)
	IFlightMode** flightModesArray; // array of all added flight mode pointers
	uint8_t amtOfFlightModes = 0; // actual amt of flight modes in the array
	uint8_t flightModesArraySize = 0; // size of the array in memory

	FC_ObjectTasker& tasker; // reference to the global tasker


private:
	void execute() override;
};


#endif

