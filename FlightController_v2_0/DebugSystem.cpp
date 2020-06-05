#include "DebugSystem.h"


DebugSystem::DebugSystem(HardwareSerial* debugSerial)
{
	this->debugSerial = debugSerial;
}


void DebugSystem::begin(uint32_t baud)
{
	debugEnabledFlag = true;
	debugSerial->begin(baud);
}


void DebugSystem::enable()
{
	debugEnabledFlag = true;
}


void DebugSystem::disable()
{
	debugEnabledFlag = false;
}


void DebugSystem::printHeader(const char* header)
{
	if (!debugEnabledFlag)
		return;

	debugSerial->print(" >> ");
	debugSerial->print(header);
	debugSerial->println(" <<");
}


void DebugSystem::printWarning(const char* message)
{
	if (!debugEnabledFlag)
		return;

	debugSerial->print(" !!> ");
	debugSerial->print(message);
	debugSerial->print(" <!!");
}


void DebugSystem::print(const char* text)
{
	if (!debugEnabledFlag)
		return;

	debugSerial->print(text);
}


void DebugSystem::println(const char* text)
{
	if (!debugEnabledFlag)
		return;

	debugSerial->println(text);
}

