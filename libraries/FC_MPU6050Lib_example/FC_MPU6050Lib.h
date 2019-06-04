// FC_MPU6050Lib.h
/*
    Created:	04/06/2019
    Author:     Jan Wielgus
	
	This program is based on the YMFC-32 made by Joop Brokking
	Link: http://www.brokking.net/YMFC-32_auto_downloads.html
*/

#ifndef _FC_MPU6050LIB_h
#define _FC_MPU6050LIB_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Wire.h>


class FC_MPU6050Lib
{
 public:
	struct vector3
	{              // IN GYRO
		int16_t x; // pitch
		int16_t y; // roll
		int16_t z; // yaw
	};
	
	FC_MPU6050Lib();
	bool initialize();
	void setFastClock();
	void read6AxisMotion();
	vector3& getAcceleration();
	vector3& getRotation();
	int16_t getTemperature();
	void calibrateGyro();             // Whole process last about 8 seconds!!
	
	
 private:
	static const uint8_t MPU6050_Address = 0x68;
	 
	vector3 acceleration;
	vector3 rotation;
	int16_t temperature;
	 
	// calibration data
	struct
	{
		 int16_t xPitch;
		 int16_t yRoll;
		 int16_t zYaw;
	} gyroCalVal;
};


#endif

