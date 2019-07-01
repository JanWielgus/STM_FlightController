// ControlSticks.h

#ifndef _CONTROLSTICKS_h
#define _CONTROLSTICKS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


namespace ctrlStk // control stick
{
	uint16_t stickValuesArr[4]; // All values there are from -500 to 500
	
	void stickInitiazlize()
	{
		
	}
	
	void readStickValues()
	{
		
	}
	
	
	
	
	
	
}



#endif

