// 
// 
// 

#include "HMC5883L_Raw.h"



HMC5883L_Raw::HMC5883L_Raw()
{
	rawData = { 0, 0, 0 };
	normData = { 0, 0, 0 };
	offset = { 0, 0, 0 };
}


bool HMC5883L_Raw::initialize()
{
	// Check if the compass is responding
	Wire.beginTransmission(HMC5883L_Address);
	if (Wire.endTransmission() != 0)
	{
		// Cannot connect with the HMC5883L

		return false;
	}

	// Device setup
	Wire.beginTransmission(HMC5883L_Address);
	Wire.write(0x00);                            // We want to write to the Configuration Register A (00 hex).
	Wire.write(0x78);                            // Set the Configuration Register A bits as 01111000 to set sample rate (average of 8 at 75Hz).
	Wire.write(0x20);                            // Set the Configuration Register B bits as 00100000 to set the gain at +/-1.3Ga.
	Wire.write(0x00);                            // Set the Mode Register bits as 00000000 to set Continues-Measurement Mode.
	Wire.endTransmission();

	return true;
}


void HMC5883L_Raw::readRaw()
{
	// Read raw data
	Wire.beginTransmission(HMC5883L_Address);
	Wire.write(0x03);
	Wire.endTransmission();

	Wire.requestFrom(HMC5883L_Address, 6);
	rawData.x = Wire.read() << 8 | Wire.read();
	rawData.z = Wire.read() << 8 | Wire.read();
	rawData.y = Wire.read() << 8 | Wire.read();

	// Apply offset
	rawData.x -= offset.x;
	rawData.y -= offset.y;
	rawData.z -= offset.z;
}


vector3Int16& HMC5883L_Raw::getRaw()
{
	return rawData;
}


vector3Float& HMC5883L_Raw::getNormalized()
{
	normData.y = (float)rawData.y * compassNormalizeMultiplier;
	normData.z = (float)rawData.z * compassNormalizeMultiplier;
	normData.x = (float)rawData.x * compassNormalizeMultiplier;
	return normData;
}


void HMC5883L_Raw::calibrate(uint16_t duration)
{
	vector3Int16 mins;
	vector3Int16 maxs;

	// Initial values for searching for minimum and maximum values
	readRaw();
	mins.x = maxs.x = rawData.x;
	mins.y = maxs.y = rawData.y;
	mins.z = maxs.z = rawData.z;

	uint32_t millisEndTime = millis() + (1000 * duration);
	while (millis() < millisEndTime)
	{
		readRaw();

		// Detect minimum and maximum values
		if (rawData.x < mins.x)
			mins.x = rawData.x;
		else if (rawData.x > maxs.x)
			maxs.x = rawData.x;

		if (rawData.y < mins.y)
			mins.y = rawData.y;
		else if (rawData.y > maxs.y)
			maxs.y = rawData.y;

		if (rawData.z < mins.z)
			mins.z = rawData.z;
		else if (rawData.z > maxs.z)
			maxs.z = rawData.z;

		// Change offset to the new values
		// += because raw values was measured with old offset
		offset.x += (double(maxs.x + mins.x) / 2.f) + 0.5f;
		offset.y += (double(maxs.y + mins.y) / 2.f) + 0.5f;
		offset.z += (double(maxs.z + mins.z) / 2.f) + 0.5f;

		delayMicroseconds(13333); // simulate 75Hz refresh rate
	}
}


const vector3Int16& HMC5883L_Raw::getCompassOffset()
{
	return offset;
}


void HMC5883L_Raw::setCompassOffset(int16_t offX, int16_t offY, int16_t offZ)
{
	offset.x = offX;
	offset.y = offY;
	offset.z = offZ;
}
