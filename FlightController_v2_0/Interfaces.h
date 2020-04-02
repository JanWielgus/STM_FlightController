// Interfaces.h

#ifndef _INTERFACES_h
#define _INTERFACES_h

#include "SharedDataTypes.h"

// Declaration of all interfaces
class IVirtualPiltot;
class IFlightMode;



class IVirtualPiltot
{
public:
	virtual ~IVirtualPiltot() {}
	virtual void runVirtualPilot() = 0;
	virtual bool addFlightMode(IFlightMode* flightModeToAdd) = 0;
	virtual bool setFlightMode(FlightModeType flightModeToSet) = 0;
	virtual FlightModeType getCurrentFlightModeType() = 0;
};



class IFlightMode
{
public:
	virtual void run() = 0;
	virtual void reset() = 0;
	virtual virtualSticksType* getVirtualSticks() = 0;
	virtual bool checkifRelated(IFlightMode* toCheck) = 0;
	virtual FlightModeType getType() = 0;
};


#endif

