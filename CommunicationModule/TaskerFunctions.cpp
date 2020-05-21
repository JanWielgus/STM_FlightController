// 
// 
// 

#include "TaskerFunctions.h"
#include "config.h"
#include "Storage.h"

using namespace Storage;


void addTaskerFunctionsToTasker()
{
	// Wifi communication task is automatically added after successful connection with router

	tasker.addTask(new TaskerFunction::OneHertzUpdate, 1000000L, 0);
}



namespace TaskerFunction
{
	void OneHertzUpdate::execute()
	{
		// builting diode indicate wifi connection
		digitalWrite(LED_BUILTIN, !wifiComm.isConnected());
		
		//Serial.println(Storage::wifiComm.isConnected());

	}
}


