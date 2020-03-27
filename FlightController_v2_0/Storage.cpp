#include "Storage.h"
#include "config.h"

using namespace config;


namespace Storage
{
	// Objects
	FC_ObjectTasker tasker(MaxAmtOfTaskerTasks);
	FC_TaskPlanner taskPlanner(MaxAmtOfTaskPlannerTasks);
	FC_MainCommunication com(&Serial1, 45);
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


	// Used flight modes
	StabilizeFlightMode stabilizeFlightMode;
	AltHoldFlightMode altHoldFlightMode(&stabilizeFlightMode);
	PosHoldFlightMode posHoldFlightMode(&altHoldFlightMode);


	// VirtualPilot object
	VirtualPilot virtualPilot(&tasker);


	// Global sensor readings
	FC_MPU6050Lib::vector3Float angle = { 0, 0, 0 }; // X and Y angles
	float heading = 0;


	// PID controllers results
	int16_t lastPID_LevelX_value;
	int16_t lastPID_LevelY_value;
	int16_t lastPID_Yaw_value;
	int16_t lastPID_AltHold_value;
}


