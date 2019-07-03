/*
 * ControlStick.h
 *
 * Created: 01/07/2019
 *  Author: Jan Wielgus
 */ 


#ifndef _FC_CONTROLSTICK_h
#define _FC_CONTROLSTICK_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <FC_EVA_Filter.h>


class FC_ControlStick
{
 public:
	FC_ControlStick();
	void setInputProperties(uint8_t pin, bool reverseFlag, uint16_t minRawVal = 0, uint16_t maxRawVal = 1023);
		// pin
		// reverseFlag - true -> need to reverse the value
		// min/maxRawVal - readings of the extreme stick tilts
	
	void setOutputValueProperties(int16_t minFinalVal, int16_t maxFinalVal,
								int16_t rawCenterValue, uint8_t nearZeroDeadZone=0);
		// min/maxFinalVal - range of the calculated output value ( getValue() ) (range have to contain 0)
		// rawCenterValue - raw value in the center
		// nearZeroDeadZone - range where despite stick movement result value is 0
	
	void readValue(); // need to be called in the specific frequency
	uint16_t getRawValue();
	int16_t getValue();
	void setFilterIntensity(uint8_t intensity); // 0-99, 0 - no filtering, 99- huge filtering
	void setRawBorderValues(uint16_t minVal, uint16_t maxVal);
	void setDeadZone(uint8_t deadZone);
 
 private:
	FC_EVA_Filter filter;
	
	int16_t rawValue;
	int16_t value;
	
	uint8_t pin;
	
	struct
	{
		uint16_t minRawValue;
		uint16_t maxRawValue;
		bool reverseFlag; // need to reverse the raw value
	} calib;
	
	struct
	{
		int16_t minFinalValue;
		int16_t maxFinalValue;
		int16_t rawCenterValue;
		uint8_t deadZone;
	} outputProperties;
	
};


#endif

