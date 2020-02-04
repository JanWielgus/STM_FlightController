#include "MeasureTime.h"

uint32_t MeasureTime::startTime = 0;
uint32_t MeasureTime::duration = 0;


void MeasureTime::start()
{
	startTime = micros();
}


void MeasureTime::end()
{
	duration = micros() - startTime;
}


uint16_t MeasureTime::getDuration()
{
	return duration;
}
