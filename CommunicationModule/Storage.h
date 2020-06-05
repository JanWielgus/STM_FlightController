// Storage.h

#ifndef _STORAGE_h
#define _STORAGE_h

#include "arduino.h"
#include <FC_ObjectTasker.h>
#include "FC_ESP8266_WiFiComm.h"
#include <FC_SerialCommBase.h>
#include "config.h"


namespace Storage
{
	extern FC_ObjectTasker tasker;
	extern FC_ESP8266_WiFiComm WiFiComm;
	extern FC_SerialCommBase serialComm;
}

#endif

