#ifndef MEASURETIME_H_
#define MEASURETIME_H_

#include <Arduino.h>

class MeasureTime
{
private:
	static uint32_t startTime;
	static uint32_t duration;

public:
	static void start()
	{
		startTime = micros();
	}

	static void end()
	{
		duration = micros() - startTime;
	}

	static uint16_t getDuration()
	{
		return duration;
	}

};

uint32_t MeasureTime::startTime = 0;
uint32_t MeasureTime::duration = 0;

#endif /* MEASURETIME_H_ */

