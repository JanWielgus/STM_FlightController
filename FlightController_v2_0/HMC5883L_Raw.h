// HMC5883L_Raw.h

#ifndef _HMC5883L_RAW_h
#define _HMC5883L_RAW_h

#include "arduino.h"
#include <Wire.h>

#include "SharedDataTypes.h" // vector3Int16


class HMC5883L_Raw
{
public:
	HMC5883L_Raw();
	bool initialize();
	void readRaw();
	vector3Int16& getRaw();
	vector3Float& getNormalized();
	void calibrate(uint16_t duration = 30); // in seconds
	const vector3Int16& getCompassOffset();
	void setCompassOffset(int16_t offX, int16_t offY, int16_t offZ);



private:
	// Raw readings
	vector3Int16 rawData;
	vector3Float normData; // updated in getter

	// Calibration values
	vector3Int16 offset;

	// Normalize multiplier
	const float compassNormalizeMultiplier = 0.92f;

	static const uint8_t HMC5883L_Address = 0x1E;
};


#endif

