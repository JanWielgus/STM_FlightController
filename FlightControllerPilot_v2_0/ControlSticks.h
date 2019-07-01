/*
 * ControlSticks.h
 *
 * Created: 01/07/2019
 *  Author: Jan Wielgus
 */ 

#ifndef _CONTROLSTICKS_h
#define _CONTROLSTICKS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


namespace ctrlStk // control stick
{
	// steer pins
	struct
	{
		uint8_t throttle;
		uint8_t rotate;
		uint8_t tiltTB;
		uint8_t tiltLR;
	} pin;
	
	pin.throttle = A0;
	pin.rotate = A1;
	pin.tiltTB = A2;
	pin.tiltLR = A3;
	
	
	uint16_t stickValuesArr[4]; // All values there are from -500 to 500
	
	void stickInitiazlize()
	{
		
	}
	
	void readStickValues()
	{
		
	}
	
	
	
	
	
	
}



#endif

