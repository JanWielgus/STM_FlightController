/*
    Created:	04/06/2019
    Author:     Jan Wielgus
*/
// 
// 
// 

#include "FC_MPU6050Lib.h"


FC_MPU6050Lib::FC_MPU6050Lib()
{
	rawAcceleration.x = 0;
	rawAcceleration.y = 0;
	rawAcceleration.z = 0;
	
	rawRotation.x = 0;
	rawRotation.y = 0;
	rawRotation.z = 0;
	
	gyroCalVal.xPitch = 0;
	gyroCalVal.yRoll = 0;
	gyroCalVal.zYaw = 0;
}


bool FC_MPU6050Lib::initialize()
{
	Wire.begin();
	
	// Check if the MPU-6050 is responding
	Wire.beginTransmission(MPU6050_Address);
	if (Wire.endTransmission() != 0)
	{
		// Cannot connect with the MPU-6050
		
		return false;
	}
	
	
	// Device setup
	Wire.beginTransmission(MPU6050_Address);                     //Start communication with the MPU-6050.
	Wire.write(0x6B);                                            //We want to write to the PWR_MGMT_1 register (6B hex).
	Wire.write(0x00);                                            //Set the register bits as 00000000 to activate the gyro.
	Wire.endTransmission();                                      //End the transmission with the gyro.

	Wire.beginTransmission(MPU6050_Address);                     //Start communication with the MPU-6050.
	Wire.write(0x1B);                                            //We want to write to the GYRO_CONFIG register (1B hex).
	Wire.write(0x08);                                            //Set the register bits as 00001000 (500dps full scale).
	Wire.endTransmission();                                      //End the transmission with the gyro.

	Wire.beginTransmission(MPU6050_Address);                     //Start communication with the MPU-6050.
	Wire.write(0x1C);                                            //We want to write to the ACCEL_CONFIG register (1A hex).
	Wire.write(0x10);                                            //Set the register bits as 00010000 (+/- 8g full scale range).
	Wire.endTransmission();                                      //End the transmission with the gyro.

	Wire.beginTransmission(MPU6050_Address);                     //Start communication with the MPU-6050.
	Wire.write(0x1A);                                            //We want to write to the CONFIG register (1A hex).
	Wire.write(0x03);                                            //Set the register bits as 00000011 (Set Digital Low Pass Filter to ~43Hz).
	Wire.endTransmission();                                      //End the transmission with the gyro.
	
	
	// Set start position to angles got from the gyro
	int samples = 50;
	for (int i=0; i<samples; i++)
	{
		read6AxisMotion();
		getAccAngles();
		fusedAngle.x += accAngle.x;
		fusedAngle.y += accAngle.y;
	}
	fusedAngle.x /= samples;
	fusedAngle.y /= samples;
	
	
	return true;
}


void FC_MPU6050Lib::setFastClock()
{
	Wire.setClock(400000L);
}


void FC_MPU6050Lib::read6AxisMotion()
{
	Wire.beginTransmission(MPU6050_Address);
	Wire.write(0x3B);
	Wire.endTransmission();
	
	Wire.requestFrom(MPU6050_Address, 14);
	rawAcceleration.x = Wire.read() << 8 | Wire.read();
	rawAcceleration.y = Wire.read() << 8 | Wire.read();
	rawAcceleration.z = Wire.read() << 8 | Wire.read();
	temperature = Wire.read() << 8 | Wire.read();
	rawRotation.y = Wire.read() << 8 | Wire.read();
	rawRotation.x = Wire.read() << 8 | Wire.read();
	rawRotation.z = Wire.read() << 8 | Wire.read();
	
	rawRotation.y *= -1; // ?????
	
	// Use the calibration data
	rawRotation.x -= gyroCalVal.xPitch;
	rawRotation.y -= gyroCalVal.yRoll;
	rawRotation.z -= gyroCalVal.zYaw;
}


void FC_MPU6050Lib::calibrateGyro()
{
	// !!!!
	// Whole process last about 8 seconds !!
	// !!!!
	
	int samples = 2000;
	int32_t sumX = 0;
	int32_t sumY = 0;
	int32_t sumZ = 0;
	
	for (int i=0; i<samples; i++)
	{
		read6AxisMotion();
		
		sumX += rawRotation.x;
		sumY += rawRotation.y;
		sumZ += rawRotation.z;
		
		delay(4); // simulate 250Hz loop
	}
	
	gyroCalVal.xPitch = sumX/samples;
	gyroCalVal.yRoll = sumY/samples;
	gyroCalVal.zYaw = sumZ/samples;
}


void FC_MPU6050Lib::setGyroFusionMultiplier(float mpr)
{
	GyroFusionMultiplier = mpr;
	AccFusionMultiplier = 1.0-mpr;
}


FC_MPU6050Lib::vector3Int& FC_MPU6050Lib::getRawAcceleration()
{
	return rawAcceleration;
}


FC_MPU6050Lib::vector3Int& FC_MPU6050Lib::getRawRotation()
{
	return rawRotation;
}


int16_t FC_MPU6050Lib::getTemperature()
{
	return temperature;
}


FC_MPU6050Lib::vector3Float& FC_MPU6050Lib::getAccAngles()
{
	static int32_t accTotalVector;
	
	accTotalVector = sqrt((rawAcceleration.x * rawAcceleration.x) +
							(rawAcceleration.y * rawAcceleration.y) +
							(rawAcceleration.z * rawAcceleration.z));
	
	if (abs(rawAcceleration.x) < accTotalVector)
	{
		accAngle.x = asin((float)rawAcceleration.x / accTotalVector) * 57.296;
	}
	
	if (abs(rawAcceleration.y) < accTotalVector)
	{
		accAngle.y = asin((float)rawAcceleration.y / accTotalVector) * 57.296;
	}
	
	accAngle.x *= -1;
	accAngle.y *= -1;
	
	return accAngle;
}


FC_MPU6050Lib::vector3Float& FC_MPU6050Lib::getFusedAngles(uint16_t freq, float compass)
{
	//Gyro angle calculations
	//0.0000611 = 1 / (250Hz / 65.5)
	// !!!!! ONLY AT 250Hz   !!!!
	
	// For 250Hz values are precalculated, so the program works faster
	if (freq == 250)
	{
		// X & Y axis
		fusedAngle.x += (float)rawRotation.x * 0.0000611;
		fusedAngle.y += (float)rawRotation.y * 0.0000611;
		
		//0.000001066 = 0.0000611 * (3.142(PI) / 180degr) The Arduino sin function is in radians and not degrees.
		fusedAngle.x += fusedAngle.y * sin((float)rawRotation.z * 0.000001066);
		fusedAngle.y -= fusedAngle.x * sin((float)rawRotation.z * 0.000001066);
		
		// Z axis
		fusedAngle.z += (float)rawRotation.z * 0.0000611;
		if(fusedAngle.z < 0.0)
			fusedAngle.z += 360.0;
		else if (fusedAngle.z >= 360.0)
			fusedAngle.z -= 360.0;
		// USE COMPASS DATA IF PROVIDED (if not, compass is == -1)  !!!!   <<<-----
	}
	else // frequency is different than 250
	{
		// formula is at the beginning
	}
	
	getAccAngles();
	
	
	// Make fusion with accelerometer data
	fusedAngle.x = fusedAngle.x * GyroFusionMultiplier + accAngle.x * AccFusionMultiplier;
	fusedAngle.y = fusedAngle.y * GyroFusionMultiplier + accAngle.y * AccFusionMultiplier;
	
	
	return fusedAngle;
}


