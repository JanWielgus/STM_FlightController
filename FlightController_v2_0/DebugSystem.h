// DebugSystem.h

#ifndef _DEBUGSYSTEM_h
#define _DEBUGSYSTEM_h

#include "Arduino.h"

class DebugSystem
{
	typedef uint32 uint32_t;

public:

	DebugSystem(HardwareSerial* debugSerial);
	void begin(uint32_t baud);
	void enable(); // show everything on the debugSerial
	void disable(); // do not show anything after using a method
	void printHeader(const char* header);
	void printWarning(const char* message);
	void print(const char* text);
	void println(const char* text);

private:
	bool debugEnabledFlag = false;
	HardwareSerial* debugSerial;
};

#endif

