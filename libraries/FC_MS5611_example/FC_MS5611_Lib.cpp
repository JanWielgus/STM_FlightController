// Based on the Joop Brokking YMFC-32 Flight Controller code
// 
// 

#include "FC_MS5611_Lib.h"


FC_MS5611_Lib::FC_MS5611_Lib()
{
	
}


bool FC_MS5611_Lib::initialize(bool needToBeginWire_flag)
{
	if (needToBeginWire_flag)
		Wire.begin();
	
	// Check if the baro is responding
	Wire.beginTransmission(MS5611_Address);
	if (Wire.endTransmission() != 0)
	{
		// Cannot connect with the MS5611
		
		return false;
	}
	
	
	// Device setup
		//For calculating the pressure the 6 calibration values need to be polled from the MS5611.
		//These 2 byte values are stored in the memory location 0xA2 and up.
	for (int i=1; i<=6; i++)
	{
		Wire.beginTransmission(MS5611_Address);
		Wire.write(0xA0 + i*2);
		Wire.endTransmission();

		Wire.requestFrom(MS5611_Address, 2);
		C[i] = Wire.read() << 8 | Wire.read();
	}
	
	// Pre-calculate some values
	OFF_C2 = C[2] * pow(2, 16);
	SENS_C1 = C[1] * pow(2, 15);
	
	
	// The MS5611 needs a few readings to stabilize
	// Read pressure for 400ms
	uint32_t readingEndTime = millis() + 400;
	while (millis() < readingEndTime)
	{
		// as fast as possible
		runBarometer();
	}
}


void FC_MS5611_Lib::setFastClock()
{
	Wire.setClock(400000L);
}


float FC_MS5611_Lib::getPressure()
{
	return pressure;
}


void FC_MS5611_Lib::runBarometer()
{
	taskPlanner.runPlanner();
}




void FC_MS5611_Lib::requestPressureFromDevice()
{
	// Request pressure data
	Wire.beginTransmission(MS5611_Address);
	Wire.write(0x48);
	Wire.endTransmission();
}


void FC_MS5611_Lib::getRawPressureFromDevice()
{
	// Get the pressure data
	Wire.beginTransmission(MS5611_Address);
	Wire.write(0x00);
	Wire.endTransmission();
	Wire.requestFrom(MS5611_Address, 3);
	rawPressure = Wire.read() << 16 | Wire.read() << 8 | Wire.read();
}


void FC_MS5611_Lib::requestTemperatureFromDevice()
{
	// Request temperature data
	Wire.beginTransmission(MS5611_Address);
	Wire.write(0x58);
	Wire.endTransmission();
}


void FC_MS5611_Lib::getRawTemperatreFromDevice()
{
	// Get the temperature data
	Wire.beginTransmission(MS5611_Address);
	Wire.write(0x00);
	Wire.endTransmission();
	Wire.requestFrom(MS5611_Address, 3);
	rawTemperature = Wire.read() << 16 | Wire.read() << 8 | Wire.read();
}


void FC_MS5611_Lib::calculatePressureAndTemperatureFromRawData()
{
	// Calculate pressure as explained in the datasheet of the MS-5611.
	dT = C[5];
	dT <<= 8;
	dT *= -1;
	dT += rawTemperature;
	OFF = OFF_C2 + ((int64_t)dT * (int64_t)C[4]) / pow(2, 7);
	SENS = SENS_C1 + ((int64_t)dT * (int64_t)C[3]) / pow(2, 8);
	intPressure = ((rawPressure * SENS) / pow(2, 21) - OFF) / pow(2, 15);
	
	pressure = (float)intPressure / 100;
	
	
	///////////////
	///
	// Do some other calculations
	///
	///////////////
}


