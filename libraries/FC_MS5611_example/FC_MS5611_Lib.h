// FC_MS5611_Lib.h
// Based on the Joop Brokking YMFC-32 Flight Controller code
//

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
	void handleBaroData(); // execute this function at 250Hz
	float getPressure(); // new pressure value is about 80 times per second
	
	
 private:
	void requestPressureFromDevice();
	void getRawPressureFromDevice(); // need to request first!
	void requestTemperatureFromDevice();
	void getRawTemperatreFromDevice(); // need to request first!
	
	
	
	
 private:
	static const uint8_t MS5611_Address = 0x77;
	static const uint8_t AFTER_REQUEST_WAIT_TIME = 8;
	
	
	// Device calibration values
	uint16_t C[6];
	int64_t OFF, OFF_C2, SENS, SENS_C1, P;
	
	
	
	// readings
	uint32_t rawPressure;
	uint32_t rawTemperature;
	float pressure;
};



#endif

