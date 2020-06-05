// 
// 
// 

#include "TaskerFunctions.h"
#include "config.h"
#include "Storage.h"


void addTaskerFunctionsToTasker()
{
	// Wifi communication task is automatically added after successful connection with router

	Storage::tasker.addTask(new TaskerFunction::OneHertzUpdate, 1000000L, 0);
	Storage::tasker.addTask(new TaskerFunction::UpdateCommunicationRelaying, 4000L, 0); // 250Hz
}



namespace TaskerFunction
{
	void OneHertzUpdate::execute()
	{
		// builting diode indicate wifi connection
		digitalWrite(LED_BUILTIN, !Storage::WiFiComm.isConnected());
		
		//Serial.println(Storage::wifiComm.isConnected());
	}


	void UpdateCommunicationRelaying::execute()
	{
		DataBuffer wifiReceivedData = Storage::WiFiComm.receiveNextData();

		// transfer from wifi to serial
		if (wifiReceivedData.size > 0)
		{
			Storage::serialComm.send(wifiReceivedData.buffer, wifiReceivedData.size);
		}

		/*
		// transfer from serial to wifi
		if (Storage::serialComm.available())
		{
			DataBuffer serialReceivedData = Storage::serialComm.receiveNextData();

			if (serialReceivedData.size > 0)
				Storage::WiFiComm.send(serialReceivedData.buffer, serialReceivedData.size);
		}*/
	}
}


