// WiFiCommunication.h

#ifndef _WIFICOMMUNICATION_h
#define _WIFICOMMUNICATION_h

#include "arduino.h"
#include <FC_Task.h>
#include <FC_TaskPlanner.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


class WiFiCommunication : public FC_Task
{
	// singleton
private:
	static WiFiCommunication* instance;
	WiFiCommunication(); // constructor
public:
	static WiFiCommunication& getInstance()
	{
		if (instance == nullptr)
			instance = new WiFiCommunication();
		return *instance;
	}



public:
	void connectWithWifiAsync(FC_TaskPlanner* taskPlanner); // asynchronically connects with wifi and add this to tasker
	void execute() override;
	bool isConnected();


private:
	uint32_t connectingAttemptStartTime; // in millis
	FC_TaskPlanner* taskPlannerPtr; // for async connecting process
	bool connectingPending = false; // prevent from starting connection several time simultaneously

	// sending and receiving buffers
	static const uint16_t MAX_PACKET_SIZE = 100;
	char receiveBuffer[MAX_PACKET_SIZE + 1];
	char sendBuffer[MAX_PACKET_SIZE + 1];

	WiFiUDP udp;
	IPAddress localIPAddress;
	friend void wifiAsyncConnecting(); // friend function used for asyns connecting
};


#endif

