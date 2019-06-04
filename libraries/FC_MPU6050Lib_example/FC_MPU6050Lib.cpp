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
	acceleration.x = 0;
	acceleration.y = 0;
	acceleration.z = 0;
	
	rotation.x = 0;
	rotation.y = 0;
	rotation.z = 0;
	
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
	acceleration.y = Wire.read() << 8 | Wire.read();
	acceleration.x = Wire.read() << 8 | Wire.read();
	acceleration.z = Wire.read() << 8 | Wire.read();
	temperature = Wire.read() << 8 | Wire.read();
	rotation.y = Wire.read() << 8 | Wire.read();
	rotation.x = Wire.read() << 8 | Wire.read();
	rotation.z = Wire.read() << 8 | Wire.read();
	
	// Invert the direction
	rotation.x *= -1;
	rotation.y *= -1;
	
	// Use the calibration data
	rotation.x -= gyroCalVal.xPitch;
	rotation.y -= gyroCalVal.yRoll;
	rotation.z -= gyroCalVal.zYaw;
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
		
		sumX += rotation.x;
		sumY += rotation.y;
		sumZ += rotation.z;
		
		delay(4); // simulate 250Hz loop
	}
	
	gyroCalVal.xPitch = sumX/samples;
	gyroCalVal.yRoll = sumY/samples;
	gyroCalVal.zYaw = sumZ/samples;
}


FC_MPU6050Lib::vector3& FC_MPU6050Lib::getAcceleration()
{
	return acceleration;
}


FC_MPU6050Lib::vector3& FC_MPU6050Lib::getRotation()
{
	return rotation;
}


int16_t FC_MPU6050Lib::getTemperature()
{
	return temperature;
}


