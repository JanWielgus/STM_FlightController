// Author: Jan Wielgus
// Date: 29.01.2019
//

#include "FC_Communication_Base.h"


FC_Communication_Base::FC_Communication_Base(Stream* serial)
{
	this->serial = serial;
}


void FC_Communication_Base::sendData(dataPacket packet)
{
	if (packet.buffer==0 || packet.size==0)
		return;
	
	uint8_t* encodeBuffer = new uint8_t[COBS::getEncodedBufferSize(packet.size)];
	
	size_t numEncoded = COBS::encode(packet.buffer, packet.size, encodeBuffer);
	
	serial->write(encodeBuffer, numEncoded);
	serial->write(PacketMarker);
	
	delete encodeBuffer;
}


/*
// Trigger pSerial.update() and check if packet have been missed
void FC_Communication_Base::receiveData()
{
	dataHandledFlag = false;
	
	pSerial.update(); // dataHandledFlag will be changed there if have been packer
	
	if (dataHandledFlag)
		missedPackets=0;
	else
		missedPackets++;
}*/



bool FC_Communication_Base::receiveData(dataPacket* packet)
{
	bool receivedDataFlag = false;
	while (serial->available() > 0)
	{
		uint8_t data = serial->read();
		
		if (data == PacketMarker)
		{
			size_t numDecoded = COBS::decode(receiveBuffer, receiveBufferIndex, decodeBuffer);
			
			packet->buffer = decodeBuffer;
			packet->size = numDecoded;
			receivedDataFlag = true;
			
			receiveBufferIndex = 0;
		}
		else
		{
			if ((receiveBufferIndex + 1) < BufferSize)
				receiveBuffer[receiveBufferIndex++] = data;
			else
			{
				// ERROR, buffer oberflow if we write.
			}
		}
	}
	
	return receivedDataFlag;
}



// update max amt of lost packets to change comState to false
void FC_Communication_Base::setMaxLostPackets(uint8_t maxLost)
{
	MaxLostPackets = maxLost;
}



bool FC_Communication_Base::checkChecksum(const uint8_t* buffer, size_t packerSize)
{
	uint8_t checksum = buffer[1];
	for (int i=2; i<packerSize; i++)
		checksum ^= buffer[i]; // xor'owanie kolejnych bajtow
		
	if (checksum == buffer[0])
		return true;
	return false;
}



uint8_t FC_Communication_Base::calcChecksum(const uint8_t* buffer, size_t packerSize)
{
	uint8_t checksum = buffer[1];
	for (int i=2; i<packerSize; i++)
		checksum ^= buffer[i]; // xor'owanie kolejnych bajtow
	
	return checksum;
}

