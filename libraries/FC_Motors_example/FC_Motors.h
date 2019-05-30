// FC_Tasker.h
//
// Author: Jan Wielgus
// Date: 23.11.2018\
//
/*
	It uses STM Timer3 so make sure that all pins will work correctly !!!
*/


#ifndef _FC_MOTORS_h
#define _FC_MOTORS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define TLmotorPin PA6      // Top left
#define TRmotorPin PA7      // Top right
#define BRmotorPin PB0      // Back left
#define BLmotorPin PB1      // Back right


class FC_Motors
{
 public:
	FC_Motors();
	void setMotorState(bool state); // (enabled/disabled because arming will be in the main program, arming should cause reset of certain values) parameters(bool: true - enabled; false - disabled)
	void setOnTR(int16_t val); // 0:1000 Top right motor
	void setOnTL(int16_t val); // 0:1000 Top left motor
	void setOnBR(int16_t val); // 0:1000 Back right motor
	void setOnBL(int16_t val); // 0:1000 Back left motor
	void forceMotorsExecution(); // reset Timer and generate pulses; if is not used cause 200Hz pulse
	
	// static does no matter because there will be only once instance of that class
	static const bool IDLE = 0;
	static const bool ENABLE = 0;
	static const bool DISABLE = 0;
	static const uint16_t MotorsDispVal = 1000; // motors value displacement
	static const uint16_t MotorMin = MotorsDispVal; // 1000, Minimum value on the motor
	static const uint16_t MotorMax = 2*MotorsDispVal; // 2000, Maximum value on the motor
	static const uint16_t MotorIdle = MotorMin; // 1000, Value when motor is idle
	
	
 private:
	bool motorsState; // enabled (true) or disabled (false)
};


#endif

