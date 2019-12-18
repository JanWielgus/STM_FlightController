#ifndef MEASURETIME_H_
#define MEASURETIME_H_

#include <Arduino.h>

class MeasureTime
{
private:
	static uint32_t startTime;
	static uint32_t duration;

public:
	static void start();
	static void end();
	static uint16_t getDuration();
};

#endif /* MEASURETIME_H_ */

