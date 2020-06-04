// 
// 
// 

#include "Storage.h"

namespace Storage
{
	FC_ObjectTasker tasker(config::MaxAmtOfTaskerTasks);
	FC_ESP8266_WiFiComm WiFiComm(config::wifiCommConfig.SSID, config::wifiCommConfig.PASS,
		config::wifiCommConfig.port, config::MaxCommPacketSize);
	FC_SerialCommBase serialComm(&Serial, config::MaxCommPacketSize);
}

