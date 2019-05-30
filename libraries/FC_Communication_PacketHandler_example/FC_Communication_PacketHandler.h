// FC_Communication_PacketHandler.h

#ifndef _FC_COMMUNICATION_PACKETHANDLER_h
#define _FC_COMMUNICATION_PACKETHANDLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <FC_Communication_Base.h>



class FC_Communication_PacketHandler
{
 public:
	void init();
};


#endif

