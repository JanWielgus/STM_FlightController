// MPU6050_Raw.h

#ifndef _MPU6050_RAW_h
#define _MPU6050_RAW_h

#include "arduino.h"
#include <Wire.h>

#include "SharedDataTypes.h" // vector3Int16


class MPU6050_Raw
{
public:
	MPU6050_Raw();
	bool initialize(); // do not begin Wire
	void readRawData();
	vector3Int16& getRawAcceleration();
	vector3Int16& getRawRotation();
	vector3Int16& getNormalizedAcceleration();
	vector3Int16& getNormalizedRotation();
	int16_t getTemperature();
	void calibrateAccelerometer(uint16_t samplesToAverage = 250);
	void calibrateGyroscope(uint16_t samplesToAverage = 2000);
	const vector3Int16& getAccOffsetValues();
	const vector3Int16& getGyroOffsetValues();
	void setAccOffset(int16_t offX, int16_t offY, int16_t offZ);
	void setGyroOffset(int16_t offX, int16_t offY, int16_t offZ);
	void enableCompassBypass();


private:
	// Raw readings
	vector3Int16 rawAcceleration;
	vector3Int16 rawRotation;
	vector3Int16 normAcceleration; // updated in getter
	vector3Int16 normRotation; // updated in getter
	int16_t temperature;

	// Calibration values
	vector3Int16 accOffset;
	vector3Int16 gyroOffset;

	// Normalize multipliters
	const float accNormalizeMultiplier = 0.0002441406f; // = 1 / 4096
	const float gyroNormalizeMultiplier = 0.01526717f; // = 1 / 65.5
	const float acc1G_value = 4096; // (from datasheet)

	static const uint8_t MPU6050_Address = 0x68;
};


#endif

