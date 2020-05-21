// 
// 
// 

#include "WiFiCommunication.h"
#include "Storage.h"


WiFiCommunication* WiFiCommunication::instance = nullptr;

// Async connecting function definition
void wifiAsyncConnecting();


WiFiCommunication::WiFiCommunication()
{

}


void WiFiCommunication::connectWithWifiAsync(FC_TaskPlanner* taskPlanner)
{
	if (connectingPending)
		return;

	WiFi.mode(WIFI_STA);
	WiFi.begin(config::wifiComm.SSID, config::wifiComm.PASS);

	// begin async connecting
	connectingAttemptStartTime = millis();
	taskPlannerPtr = taskPlanner;
	connectingPending = true;
	wifiAsyncConnecting();
}


void wifiAsyncConnecting()
{
	WiFiCommunication& andCommInst = WiFiCommunication::getInstance();
	if (WiFi.status() == WL_CONNECTED)
	{
		// wifi connection established
		andCommInst.localIPAddress = WiFi.localIP();
		andCommInst.udp.begin(config::wifiComm.port);

		// add new tasker task
		Storage::tasker.addTask(&andCommInst, config::wifiComm.receivingInterval, 0);

		andCommInst.connectingPending = false;
	}
	else
	{
		// not connected, check if not timeout and try again
		if (millis() < andCommInst.connectingAttemptStartTime + ((uint32_t)config::wifiComm.connectingTimeTimeout * 1000))
			andCommInst.taskPlannerPtr->scheduleTask(wifiAsyncConnecting, 500); // try to reconnect in 500 ms
		else
			andCommInst.connectingPending = false;
	}
}


void WiFiCommunication::execute()
{
	// TODO: check if work with available() before (but should work as it is)
	int packetSize = udp.parsePacket();

	if (packetSize)
	{
		int recPacketSize = udp.read(receiveBuffer, MAX_PACKET_SIZE);


		// Send immediately using packetSerial
		Storage::comm.dpToSend.buffer = (uint8_t*)receiveBuffer;
		Storage::comm.dpToSend.size = recPacketSize;
		Storage::comm.dpToSend.buffer[0] = Storage::comm.calcChecksum();
		Storage::comm.sendData();

		/*
		for (int i = 0; i < recPacketSize; i++)
		{
			Serial.print((uint8_t)receiveBuffer[i]);
			Serial.print('\t');
		}
		Serial.println();*/
		


		/*
		// !!!
		// TODO: Do something with received data
		if (receiveBuffer[0] == 0 && recPacketSize == 18) // if packet ID == 0 and size match
		{
			SendData::tunedControllerID = receiveBuffer[1];
			uint8_t* temp = (uint8_t*)receiveBuffer; // the same array but of uint8_t type
			SendData::tunedPID_values.P = uint8ArrayToInt(temp + 2) / 100.f;
			SendData::tunedPID_values.I = uint8ArrayToInt(temp + 6) / 100.f;
			SendData::tunedPID_values.D = uint8ArrayToInt(temp + 10) / 100.f;
			SendData::tunedPID_values.I_max = uint8ArrayToInt(temp + 14);

			// send new PID values to drone
			Storage::comm.sendDataPacket(&SendData::DP_PID_params);
		}*/
	}
}


bool WiFiCommunication::isConnected()
{
	return WiFi.status() == WL_CONNECTED;
}


