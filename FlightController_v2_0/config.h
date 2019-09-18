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
	// pins
	struct 
	{
		/* not used in the code
		// motors
		const uint8_t mTL = PA6;
		const uint8_t mTR = PA7;
		const uint8_t mBR = PB0;
		const uint8_t mBL = PB1;
		*/
		
		
		// diodes
		const uint8_t redDiode = PB15;
		const uint8_t blueDiode = PB14;
		
		
		// radio module
		const uint8_t m0pin = PA1;
		const uint8_t m1pin = PA0;
		const uint8_t aux = PB5;
		
		
	} pin;
	
	
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
	const uint16_t ZeroG_throttle = 500; // throttle value when motors power compensate the gravity force
	const uint8_t AltHoldMaxAddedThrottle = 90;
	const uint8_t AltHoldStickDeadZone = 90; // value from the stick center which cause altitude change
	const uint8_t PosHoldStickDeadZone = 60; // value from the stick center which cause position change


	// other
	const uint8_t ZeroActionThrottle = 20;
}




#endif /* CONFIG_H_ */