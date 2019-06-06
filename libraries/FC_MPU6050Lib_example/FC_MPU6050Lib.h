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
	struct vector3Int
	{              // IN GYRO
		int16_t x; // pitch
		int16_t y; // roll
		int16_t z; // yaw
	};
	
	struct vector3Float
	{            // IN GYRO
		float x; // pitch
		float y; // roll
		float z; // yaw
	};
	
	FC_MPU6050Lib();
	bool initialize();
	void setFastClock();
	void read6AxisMotion();
	vector3Int& getRawAcceleration();
	vector3Int& getRawRotation();
	int16_t getTemperature();
	vector3Float& getAccAngles();                                        // angles calculated from the accelerometer
	//vector3& getGyroAngles();                                          // (useless) angles calculated from the gyro (rotation)
	vector3Float& getFusedAngles(float compass=-1);
	void calibrateGyro(int samples = 2000);                              // Whole process last about 8 seconds!!
	void setGyroFusionMultiplier(float);
	void setCalculationsFrequency(uint16_t freq=250);
	
	
 private:
	static const uint8_t MPU6050_Address = 0x68;
	float GyroFusionMultiplier = 0.9996; // default gyro fusion multiplier
	float AccFusionMultiplier = 0.0004;  // default accelerometer fusion multiplier
	
	// gyro angle calculations multipliers
	float Multiplier1;
	float Multiplier2;
	 
	vector3Int rawAcceleration;
	vector3Int rawRotation;
	int16_t temperature;
	vector3Float accAngle;       // angles only form the accelerometer
	vector3Float fusedAngle;
	 
	// calibration data
	struct
	{
		 int16_t xPitch;
		 int16_t yRoll;
		 int16_t zYaw;
	} gyroCalVal;
};


#endif

