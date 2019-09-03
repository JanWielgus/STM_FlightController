// FC_MS5611_Lib.h

#ifndef _FC_MS5611_LIB_h
#define _FC_MS5611_LIB_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Wire.h>


class FC_MS5611_Lib
{
 public:
	FC_MS5611_Lib();
	bool initialize(bool needToBeginWire_flag = true);
	void setFastClock();
	void readFixedFreqBaroData(); // execute this function in 80Hz
	
	
	
 private:
	static const uint8_t MS5611_Address = 0x77;
	
	
	uint16_t hardwareCalibValuesArr[6];
};



#endif

