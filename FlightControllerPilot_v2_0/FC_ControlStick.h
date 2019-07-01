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
	FC_ControlStick(uint8 pin, bool reverseFlag, int16_t minFinalVal, int16_t maxFinalVal,
					uint16_t minRawVal = 0, uint16_t maxRawVal = 1023);
	void readValue(); // need to be called in the specific frequency
	uint16_t getRawValue();
	int16_t getValue();
	void setFilterIntensity(uint8_t intensity); // 0-99, 0 - no filtering, 99- huge filtering
	void setRawBorderValues(uint16_t minVal, uint16_t maxVal);
 
 private:
	FC_EVA_Filter filter;
	
	uint16_t rawValue;
	int16_t value;
	
	uint8_t pin;
	uint16_t minValue;
	uint16_t maxValue;
	bool reverseFlag; // need to reverse the raw value
};


#endif

