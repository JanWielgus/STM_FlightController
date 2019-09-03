// 
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
	
}


