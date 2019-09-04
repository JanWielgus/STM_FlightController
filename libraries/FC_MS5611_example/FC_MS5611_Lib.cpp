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
	for (int i=0; i<6; i++)
	{
		Wire.beginTransmission(MS5611_Address);
		Wire.write(0xA0 + (i+1)*2);
		Wire.endTransmission();
		
		Wire.requestFrom(MS5611_Address, 2);
		C[i] = Wire.read() << 8 | Wire.read();
	}
	
	// Pre-calculate some values
	OFF_C2 = C[1] * pow(2, 16);
	SENS_C1 = C[0] * pow(2, 15);
	
	// The MS5611 needs a few readings to stabilize
	for (int i=0; i<100; i++)
	{
		handleBaroData();
		delay(4); // simulate 250Hz loop
	}
}


void FC_MS5611_Lib::setFastClock()
{
	Wire.setClock(400000L);
}


void FC_MS5611_Lib::handleBaroData()
{
	
}

float FC_MS5611_Lib::getPressure()
{
	return pressure;
}


void FC_MS5611_Lib::requestPressureFromDevice()
{
	
}


void FC_MS5611_Lib::getRawPressureFromDevice()
{
	
}


void FC_MS5611_Lib::requestTemperatureFromDevice()
{
	
}


void FC_MS5611_Lib::getRawTemperatreFromDevice()
{
	
}


