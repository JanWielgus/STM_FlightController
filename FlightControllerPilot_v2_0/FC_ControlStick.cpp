// 
// 
// 

#include "FC_ControlStick.h"

FC_ControlStick::FC_ControlStick(uint8 pin, bool reverseFlag, int16_t minFinalVal, int16_t maxFinalVal,
								 uint16_t minRawVal, uint16_t maxRawVal)
	: filter(0.6)
{
	this->pin = pin;
	this->reverseFlag = reverseFlag;
	setRawBorderValues(minRawVal, maxRawVal);
}


void FC_ControlStick::readValue()
{
	rawValue = analogRead(pin);
	
	// reverse if needed
	if (reverseFlag)
		rawValue = abs(rawValue - (minRawValue + maxRawValue));
	
	// filter
	value = (int16_t)filter.updateFilter((float)rawValue);
}


uint16_t FC_ControlStick::getRawValue()
{
	return rawValue;
}


int16_t FC_ControlStick::getValue()
{
	return value;
}


void FC_ControlStick::setFilterIntensity(uint8_t intensity)
{
	intensity = constrain(intensity, 0, 99);
	filter.setFilterBeta((float)intensity / 100);
}


void FC_ControlStick::setRawBorderValues(uint16_t minVal, uint16_t maxVal)
{
	this->minRawValue = minVal;
	this->maxRawValue = maxVal;
}


