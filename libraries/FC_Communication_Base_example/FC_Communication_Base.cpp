// Author: Jan Wielgus
// Date: 29.01.2019
//

#include "FC_Communication_Base.h"


FC_Communication_Base::FC_Communication_Base(Stream* serial, uint8_t bufSize): BufferSize(bufSize)
{
	this->serial = serial;
	
	// packet to prepare is used outside the class to pack there data to send
	dpToSend->buffer = new uint8_t[bufSize];
}

FC_Communication_Base::~FC_Communication_Base()
{
	delete [] dpToSend->buffer;
}


// send data from dpToSend filled before outside the com class
void FC_Communication_Base::sendData()
{
	if (dpToSend->buffer==0 || dpToSend->size==0)
		return;
	
	uint8_t* encodeBuffer = new uint8_t[COBS::getEncodedBufferSize(dpToSend->size)];
	
	size_t numEncoded = COBS::encode(dpToSend->buffer, dpToSend->size, encodeBuffer);
	
	serial->write(encodeBuffer, numEncoded);
	serial->write(PacketMarker);
	
	delete encodeBuffer;
}



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

