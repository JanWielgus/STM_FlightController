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


namespace config
{
	const uint8_t MainFrequeny = 250; // [Hz]
	const float MainDeltaTimeInSeconds = 1.0f / (float)MainFrequeny; //  = 1/250  (250Hz)
	const uint16_t MainInterval = (uint16_t)(MainDeltaTimeInSeconds * 1000000.0); // [micro seconds] interval of main tasks

	const uint8_t MaxAmtOfTaskerTasks = 20; // tasks array size inside tasker


	// mpu6050
	struct 
	{
		// mpu6050
		//FC_MPU6050Lib::vector3Int accelerometer;
		
		// GYRO CALIBRATION VALUES
		
		
		// hmc5883l
		// Default calibration values
// 		FC_HMC5883L_Lib::vector3Int compassMin = {-503, -505, -1440};
// 		FC_HMC5883L_Lib::vector3Int compassMax = {504, 463, -330};
		FC_HMC5883L_Lib::vector3Int compassMin = {-312, -804, -2016};
		FC_HMC5883L_Lib::vector3Int compassMax = {299, -192, -978};
		
		
		
	} calibVal;


	// flight modes config values
	struct
	{
		//const uint16_t ZeroG_throttle = 500; // throttle value when motors power compensate the gravity force
		uint8_t AltHoldMaxAddedThrottle = 150;
		uint16_t AltHoldMaxTotalFinal = 720;
		uint16_t AltHoldMinTotalFinal = 20;
		uint8_t AltHoldStickDeadZone = 90; // value from the stick center which cause altitude change
		uint8_t PosHoldStickDeadZone = 60; // value from the stick center which cause position change
	} const flModeConfig; // flight mode config


	// PID default values
	struct pidSet
	{
		float p;
		float i;
		uint8_t imax;
		float d;
	};

	struct
	{
		const pidSet leveling = { 2.14f, 2.5f, 110, 0.64f };
		const pidSet yaw = { 3.8f, 1.2f, 90, 0.8f };
		const pidSet altHold = { 0, 0, 0, 0 };
	}defPID;


	// other
	const uint8_t ZeroActionThrottle = 20;
	const float CutOffAngle = 46; // Angle from which motors are immediately disabled



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