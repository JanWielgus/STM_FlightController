/*
 * config.h
 *
 * Created: 21/07/2019 11:32:35
 *  Author: Jan Wielgus
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

#include <FC_MPU6050Lib.h>
#include <FC_HMC5883L_Lib.h>
#include "SharedDataTypes.h"


namespace config
{
// Numerical constants

	const uint8_t MainFrequeny = 250; // [Hz]
	const float MainDeltaTimeInSeconds = 1.0f / (float)MainFrequeny; //  = 1/250  (250Hz)
	const uint16_t MainInterval = (uint16_t)(MainDeltaTimeInSeconds * 1000000.0); // [micro seconds] interval of main tasks

	const uint8_t MaxAmtOfTaskerTasks = 25; // tasks array size inside tasker
	const uint8_t MaxAmtOfTaskPlannerTasks = 7; // max amount of tasks planned at once

	const uint8_t MaxCommPacketBytes = 100; // communication buffer size

	const uint8_t ZeroActionMotorPower = 5; // if average motor power is below that value, some stuff could be frozeb, like stabilize PID controllers


	// Failsafe
	struct
	{
		bool CutOffMotorsIfAngleExceeded = true; // When connection stability drops below 1 motors will be cut-off
		uint8_t CutOffAngle = 60; // Angle from which motors are immediately disabled

		bool SuppressMotorsAndDisarmWhenLostSignal = true; // in 0.5 second gradually suppress motors and in the end disarm
		uint8_t ConnectionStabilityBorder = 15; // below that value and if flag above is true suppress will take place
		float suppressMultiplier = 0.7; // if failsafe in 20Hz and 0.5 suppress will take about 0.5 second to disarm

	} const failsafeSettings;


// Boolean constants

	struct
	{
		bool UseCompassInZAxisAngleCalculation = false; // Last time there was something wrong with the compass reading


	} const booleans;




// mpu6050
	struct 
	{
		// mpu6050
		//FC_MPU6050Lib::vector3Int accelerometer;
		
		// GYRO CALIBRATION VALUES
		
		
		// hmc5883l
		// Default calibration values
		FC_HMC5883L_Lib::vector3Int compassMin = {-507, -170, -1010};
		FC_HMC5883L_Lib::vector3Int compassMax = {370, 725, 152};
		
		
		
	} calibVal;

// Flight modes config
	struct
	{
		uint16_t altHoldBaseThrottle = 580; // throttle value, when motors should compensate gravity
		uint8_t altHoldThrottleCenterOffset = 100; // when throttle will be between (500-offset, 500+offset) range, pressure to hold will not be integrated
		uint16_t altHoldMinThrottle = 350;
		uint16_t altHoldMaxThrottle = 780;
	} const flightModeConfig;


// PID default values
	struct
	{
		const pidPack leveling = { 2.14f, 2.5f, 110, 0.64f };
		const pidPack yaw = { 3.8f, 1.2f, 90, 0.8f };
		const pidPack altHold = { 0, 0, 0, 0 };
	}defPID;


// pins
	struct
	{
		/* not used in the code
		// motors
		uint8_t mTL = PA6;
		uint8_t mTR = PA7;
		uint8_t mBR = PB0;
		uint8_t mBL = PB1;
		*/


		// diodes
		uint8_t redDiode = PB15;
		uint8_t blueDiode = PB14;


		// radio module
		uint8_t m0pin = PA1;
		uint8_t m1pin = PA0;
		uint8_t aux = PB5;


		uint8_t LedBuiltIn = LED_BUILTIN;

	} const pin;
}




#endif /* CONFIG_H_ */