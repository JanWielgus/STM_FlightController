#include "Storage.h"
#include "config.h"

#include "VirtualPilot.h"
#include "FlightMode.h"
#include "StabilizeFlightMode.h"
#include "AltHoldFlightMode.h"
#include "PosHoldFlightMode.h"

using namespace config;


namespace Storage
{
	// Objects
	FC_ObjectTasker tasker(MaxAmtOfTaskerTasks);
	FC_TaskPlanner taskPlanner(MaxAmtOfTaskPlannerTasks);
	FC_CommunicationHandler comm(&Serial1, MaxCommPacketBytes);
	FC_MPU6050Lib mpu;
	FC_HMC5883L_Lib compass;
	FC_MS5611_Lib baro(&taskPlanner);
	FC_Motors motors;
	DebugSystem debug(&Serial);


	// PID objects
	MyPID levelXpid(MainDeltaTimeInSeconds, defPID.leveling.p, defPID.leveling.i, defPID.leveling.d, defPID.leveling.imax);
	MyPID levelYpid(MainDeltaTimeInSeconds, defPID.leveling.p, defPID.leveling.i, defPID.leveling.d, defPID.leveling.imax);
	MyPID yawPID(MainDeltaTimeInSeconds, defPID.yaw.p, defPID.yaw.i, defPID.yaw.d, defPID.yaw.imax);
	MyPID altHoldPID(MainDeltaTimeInSeconds, defPID.altHold.p, defPID.altHold.i, defPID.altHold.d, defPID.altHold.imax);


	// VirtualPilot
	IVirtualPilot& virtualPilot = *(new VirtualPilot(&tasker));


	// Used flight modes
	IFlightMode& stabilizeFlightMode = *(new StabilizeFlightMode(&virtualPilot));
	IFlightMode& altHoldFlightMode = *(new AltHoldFlightMode(&stabilizeFlightMode, &virtualPilot));
	IFlightMode& posHoldFlightMode = *(new PosHoldFlightMode(&altHoldFlightMode, &virtualPilot));


	// Global sensor readings
	readingsType reading;

}


