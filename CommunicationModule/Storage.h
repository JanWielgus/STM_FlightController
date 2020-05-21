// Storage.h

#ifndef _STORAGE_h
#define _STORAGE_h

#include "arduino.h"
#include <FC_TaskPlanner.h>
#include <FC_ObjectTasker.h>
#include <FC_Communication_Base.h>
#include "WiFiCommunication.h"
#include "config.h"


namespace Storage
{
	extern FC_ObjectTasker tasker;
	extern FC_TaskPlanner taskPlanner;
	extern FC_Communication_Base comm;
	extern WiFiCommunication& wifiComm;
}

#endif

