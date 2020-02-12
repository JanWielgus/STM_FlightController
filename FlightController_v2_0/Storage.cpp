#include "Storage.h"
#include "config.h"

using namespace config;


namespace Storage
{
	// Objects
	FC_SimpleTasker tasker;
	FC_MainCommunication com(&Serial1, 45);
	FC_MPU6050Lib mpu;
	FC_HMC5883L_Lib compass;
	// Barometer object is created inside the library
	FC_Motors motors;
	DebugSystem debug(&Serial);


	// PID objects
	MyPID levelXpid(calculationsDeltaTime, defPID.leveling.p, defPID.leveling.i, defPID.leveling.d, defPID.leveling.imax);
	MyPID levelYpid(calculationsDeltaTime, defPID.leveling.p, defPID.leveling.i, defPID.leveling.d, defPID.leveling.imax);
	MyPID yawPID(calculationsDeltaTime, defPID.yaw.p, defPID.yaw.i, defPID.yaw.d, defPID.yaw.imax);
	MyPID altHoldPID(calculationsDeltaTime, defPID.altHold.p, defPID.altHold.i, defPID.altHold.d, defPID.altHold.imax);


	// Used flight modes
	StabilizeFlightMode stabilizeFlightMode;
	AltHoldFlightMode altHoldFlightMode(&stabilizeFlightMode);
	PosHoldFlightMode posHoldFlightMode(&altHoldFlightMode);


	// VirtualPilot object
	VirtualPilot virtualPilot(&tasker);


	// Variables
	FC_MPU6050Lib::vector3Float angle = { 0, 0, 0 }; // X and Y angles
	float heading = 0;
	float headingToHold = 0; // calculated value based on the pilot rotate stick to hold by the drone
	float pressureToHold;


	// Extrapolation variables
	int16_t previousTBvalue = 0;
	int16_t previousLRvalue = 0;
	float extrapolatedTBstick = 0;
	float extrapolatedLRstick = 0;
	FC_EVA_Filter tbFilter(0.4);
	FC_EVA_Filter lrFilter(0.4);



	// PID controllers results
	int16_t lastPID_LevelX_value;
	int16_t lastPID_LevelY_value;
	int16_t lastPID_Yaw_value;
	int16_t lastPID_AltHold_value;
}


