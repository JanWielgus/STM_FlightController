// 
// 
// 

#include "MPU6050_Raw.h"

MPU6050_Raw::MPU6050_Raw()
{
	rawAcceleration = { 0, 0, 0 };
	rawRotation = { 0, 0, 0 };
	temperature = 0;
	accOffset = { 0, 0, 0 };
	gyroOffset = { 0, 0, 0 };
}


bool MPU6050_Raw::initialize()
{
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


void MPU6050_Raw::readRawData()
{
	Wire.beginTransmission(MPU6050_Address);
	Wire.write(0x3B);
	Wire.endTransmission();

	Wire.requestFrom(MPU6050_Address, 14);
	rawAcceleration.x = Wire.read() << 8 | Wire.read();
	rawAcceleration.y = Wire.read() << 8 | Wire.read();
	rawAcceleration.z = Wire.read() << 8 | Wire.read();
	temperature = Wire.read() << 8 | Wire.read();
	rawRotation.x = Wire.read() << 8 | Wire.read();
	rawRotation.y = Wire.read() << 8 | Wire.read();
	rawRotation.z = Wire.read() << 8 | Wire.read();


	// Use calibration data
	rawAcceleration.x -= accOffset.x;
	rawAcceleration.y -= accOffset.y;
	rawAcceleration.z -= accOffset.z;
	rawRotation.x -= gyroOffset.x;
	rawRotation.y -= gyroOffset.y;
	rawRotation.z -= gyroOffset.z;


	// Temperature                    / 340
	temperature = ((float)temperature * 0.002941f + 36.53f) + 0.5f; // 0.5 to average for int
}


vector3Int16& MPU6050_Raw::getRawAcceleration()
{
	return rawAcceleration;
}


vector3Int16& MPU6050_Raw::getRawRotation()
{
	return rawRotation;
}


vector3Float& MPU6050_Raw::getNormalizedAcceleration()
{
	normAcceleration.x = (float)rawAcceleration.x * accNormalizeMultiplier;
	normAcceleration.y = (float)rawAcceleration.y * accNormalizeMultiplier;
	normAcceleration.z = (float)rawAcceleration.z * accNormalizeMultiplier;
	return normAcceleration;
}


vector3Float& MPU6050_Raw::getNormalizedRotation()
{
	normRotation.x = (float)rawRotation.x * gyroNormalizeMultiplier;
	normRotation.y = (float)rawRotation.y * gyroNormalizeMultiplier;
	normRotation.z = (float)rawRotation.z * gyroNormalizeMultiplier;
	return normRotation;
}


int16_t MPU6050_Raw::getTemperature()
{
	return temperature;
}


void MPU6050_Raw::calibrateAccelerometer(uint16_t samplesToAverage)
{
	int32_t sumX = 0;
	int32_t sumY = 0;
	int32_t sumZ = 0;

	for (int i = 0; i < samplesToAverage; i++)
	{
		readRawData();

		sumX += rawAcceleration.x;
		sumY += rawAcceleration.y;
		sumZ += rawAcceleration.z - acc1G_value;

		delay(4);
	}

	accOffset.x += ((double)sumX / samplesToAverage) + 0.5f;
	accOffset.y += ((double)sumY / samplesToAverage) + 0.5f;
	accOffset.z += ((double)sumZ / samplesToAverage) + 0.5f;
}


void MPU6050_Raw::calibrateGyroscope(uint16_t samplesToAverage)
{
	int32_t sumX = 0;
	int32_t sumY = 0;
	int32_t sumZ = 0;

	for (int i = 0; i < samplesToAverage; i++)
	{
		readRawData();

		sumX += rawRotation.x;
		sumY += rawRotation.y;
		sumZ += rawRotation.z;

		delay(4);
	}

	gyroOffset.x += ((double)sumX / samplesToAverage) + 0.5f;
	gyroOffset.y += ((double)sumY / samplesToAverage) + 0.5f;
	gyroOffset.z += ((double)sumZ / samplesToAverage) + 0.5f;
}


const vector3Int16& MPU6050_Raw::getAccOffset()
{
	return accOffset;
}


const vector3Int16& MPU6050_Raw::getGyroOffset()
{
	return gyroOffset;
}


void MPU6050_Raw::setAccOffset(int16_t offX, int16_t offY, int16_t offZ)
{
	accOffset.x = offX;
	accOffset.y = offY;
	accOffset.z = offZ;
}


void MPU6050_Raw::setGyroOffset(int16_t offX, int16_t offY, int16_t offZ)
{
	gyroOffset.x = offX;
	gyroOffset.y = offY;
	gyroOffset.z = offZ;
}


void MPU6050_Raw::enableCompassBypass()
{
	// setting I2C Master Mode disabled
	writeRegisterBit(MPU6050_Address, 0x6A, 5, false);

	// setting I2C Bypass enabled
	writeRegisterBit(MPU6050_Address, 0x37, 1, true);

	// setting Sleep disabled
	writeRegisterBit(MPU6050_Address, 0x6B, 6, false);
}





// Help funcitons for enabling compass bypass on MPU6050
uint8_t readRegister8(uint8_t address, uint8_t reg)
{
	uint8_t value;

	Wire.beginTransmission(address);
	Wire.write(reg);
	Wire.endTransmission();

	Wire.beginTransmission(address);
	Wire.requestFrom(address, 1);
	while (!Wire.available())
	{
	};

	value = Wire.read();
	Wire.endTransmission();

	return value;
}

void writeRegister8(uint8_t address, uint8_t reg, uint8_t value)
{
	Wire.beginTransmission(address);
	Wire.write(reg);
	Wire.write(value);
	Wire.endTransmission();
}

void writeRegisterBit(uint8_t address, uint8_t reg, uint8_t pos, bool state)
{
	uint8_t value;
	value = readRegister8(address, reg);

	if (state)
		value |= (1 << pos);
	else
		value &= ~(1 << pos);

	writeRegister8(address, reg, value);
}
