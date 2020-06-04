#include "Storage.h"
#include "config.h"

#include "VirtualPilot.h"
#include "FlightMode.h"
#include "UnarmedFlightMode.h"
#include "StabilizeFlightMode.h"
#include "AltHoldFlightMode.h"
#include "PosHoldFlightMode.h"

using namespace config;


namespace Storage
{
	// Objects
	FC_ObjectTasker tasker(MaxAmtOfTaskerTasks);
	FC_TaskPlanner taskPlanner(MaxAmtOfTaskPlannerTasks);
	FC_SerialCommBase lowLevelComm(&Serial2, MaxCommPacketBytes);
	FC_CommunicationHandler comm(&lowLevelComm);
	MPU6050_Raw rawMPU6050;
	HMC5883L_Raw rawHMC5883L;
	FC_MS5611_Lib baro(&taskPlanner);
	FC_Motors motors;
	DebugSystem debug(&Serial);

	// Select full AHRS or just IMU version
	//MadgwickAHRS madgwickAHRS(config::MainFrequency, 0.05); // created statically, to offload heap for dynamic allocation
	MadgwickIMU madgwickIMU(config::MainFrequency, 0.05);
	AHRS ahrs(&madgwickIMU, new NormAccelerometerAdapter,
		new NormGyroscopeAdapter, new NormMagnetometerAdapter);


	// PID objects
	MyPID levelXpid(MainDeltaTimeInSeconds, defPID.leveling.p, defPID.leveling.i, defPID.leveling.d, defPID.leveling.imax);
	MyPID levelYpid(MainDeltaTimeInSeconds, defPID.leveling.p, defPID.leveling.i, defPID.leveling.d, defPID.leveling.imax);
	MyPID yawPID(MainDeltaTimeInSeconds, defPID.yaw.p, defPID.yaw.i, defPID.yaw.d, defPID.yaw.imax);
	MyPID altHoldPID(MainDeltaTimeInSeconds, defPID.altHold.p, defPID.altHold.i, defPID.altHold.d, defPID.altHold.imax);


	// VirtualPilot
	IVirtualPilot& virtualPilot = *(new VirtualPilot(&tasker));


	// Used flight modes
	IFlightMode& unarmedFlightMode = *(new UnarmedFlightMode(&virtualPilot));
	IFlightMode& stabilizeFlightMode = *(new StabilizeFlightMode(&virtualPilot));
	IFlightMode& altHoldFlightMode = *(new AltHoldFlightMode(&stabilizeFlightMode, &virtualPilot));
	IFlightMode& posHoldFlightMode = *(new PosHoldFlightMode(&altHoldFlightMode, &virtualPilot));


	// Global sensor readings
	readingsType reading;
	virtualSticksType sticksFiltered; // filtered steering sticks values

}


