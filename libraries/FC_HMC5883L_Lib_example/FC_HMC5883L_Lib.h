// FC_HMC5883L_Lib.h
/*
    Created:	06/06/2019
    Author:     Jan Wielgus
	
	Parts of that program are based on the YMFC-32 made by Joop Brokking
	Link: http://www.brokking.net/YMFC-32_auto_downloads.html
*/


#ifndef _FC_HMC5883L_LIB_h
#define _FC_HMC5883L_LIB_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Wire.h>

// !!! Comment if you don't want to debug
#define SERIAL_CALIBRATION_DEBUG


class FC_HMC5883L_Lib
{
 public:
	struct vector3Int
	{
		int16_t x;
		int16_t y;
		int16_t z;
	};
	
	FC_HMC5883L_Lib();
	bool initialize(bool needToBeginWire_flag = true);
	void readCompassData(float angleX=-100, float angleY=-100);
	float getHeading();
	void calibrateCompass();
	void setCalibrationValues(vector3Int& minimums, vector3Int& maximums); // instead of making a full calibration, provide values from earlier calibration
	void setCompassDeclination(float declin);
	void setCalibrationDuration(uint8_t seconds); // how long time will take compass calibration
	
 private:
	static const uint8_t HMC5883L_Address = 0x1E;

	vector3Int compass_raw; // raw data read from the device
	vector3Int compass; // compass data after calculations
	float heading;
	float declination;
	uint8_t calibrationDuration; // seconds
	
	// calibration values to calculate scale and offset
	vector3Int calMins; // calibration minimum values of raw readings
	vector3Int calMaxs; // calibration maximum values of raw readings
	// offset
	vector3Int offset;
	// scale multipliers (don't know why without x)
	float scale_y;
	float scale_z;
};


#endif

