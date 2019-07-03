// 
// 
// 

#include "FC_ControlStick.h"

FC_ControlStick::FC_ControlStick()
	:filter(0.3)
{
	// Nothing here
}


void FC_ControlStick::setInputProperties(uint8_t pin, bool reverseFlag, uint16_t minRawVal, uint16_t maxRawVal)
{
	this->pin = pin;
	calib.reverseFlag = reverseFlag;
	setRawBorderValues(minRawVal, maxRawVal);
}


void FC_ControlStick::setOutputValueProperties(int16_t minFinalVal, int16_t maxFinalVal, int16_t rawCenterValue, uint8_t nearZeroDeadZone)
{
	outputProperties.minFinalValue = minFinalVal;
	outputProperties.maxFinalValue = maxFinalVal;
	outputProperties.rawCenterValue = rawCenterValue;
	setDeadZone(nearZeroDeadZone);
}


void FC_ControlStick::readValue()
{
	rawValue = analogRead(pin);
	
	// reverse if needed
	if (calib.reverseFlag)
	{
		rawValue = rawValue - (calib.minRawValue + calib.maxRawValue);
		rawValue = abs(rawValue);
	}
	
	// filter
	float temp = rawValue;
	value = (int16_t)(filter.updateFilter(temp) + 0.5);
	
	// calculate the output values
	if (value > outputProperties.rawCenterValue + outputProperties.deadZone)
	{
		value = map(value,
					outputProperties.rawCenterValue + outputProperties.deadZone, calib.maxRawValue,
					0, outputProperties.maxFinalValue);
		value = constrain(value, 0, outputProperties.maxFinalValue);
	}
	else if (value < outputProperties.rawCenterValue - outputProperties.deadZone)
	{
		value = map(value,
					calib.minRawValue, outputProperties.rawCenterValue - outputProperties.deadZone,
					outputProperties.minFinalValue, 0);
		value = constrain(value, outputProperties.minFinalValue, 0);
	}
	else
		value = 0;
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
	calib.minRawValue = minVal;
	calib.maxRawValue = maxVal;
}


void FC_ControlStick::setDeadZone(uint8_t deadZone)
{
	outputProperties.deadZone = deadZone;
}


