// Author: Jan Wielgus
// Date: 30.01.2019
//

#include "FC_Communication_Base.h"

void receive();
void send();

// example variables - data to send and reveive
uint8_t var1;
uint8_t var2;

const uint16_t MAX_MISSED_PACKETS = 5;
const uint8_t MAX_PACKET_SIZE = 50;
uint16_t missedPackets = 0;
bool comState = false;


FC_Communication_Base com(&Serial1, MAX_PACKET_SIZE);



void setup()
{
	Serial1.begin(9600);
}


void loop()
{
	receive();
	delay(500);
	send();
	delay(500);
}



void receive()
{
	bool receivedDataFlag = false;
	dataPacket dp;
	// jesli jest gotowa ramka to przekopiuj do zmiennych
	if (com.receiveData(&dp))
	{
		var1 = dp.buffer[0];
		var2 = dp.buffer[1];
		receivedDataFlag = true;
	}
	
	if (receivedDataFlag)
	{
		missedPackets = 0;
		comState = true;
	}
	else
	{
		missedPackets++;
		if (missedPackets>MAX_MISSED_PACKETS)
			comState = false;
	}
}


void send()
{
	com.dpToSend->buffer[0] = var1;
	com.dpToSend->buffer[1] = var2;
	
	com.dpToSend->size = 2; // IMPORTANT !
	
	com.sendData();
}