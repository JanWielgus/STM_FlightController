// Author: Jan Wielgus
// Date: 29.01.2019
//

#include "FC_Communication_Base.h"


FC_Communication_Base::FC_Communication_Base(Stream* serial)
{
	pSerial.setPacketHandler(this->handleReceivedData);
	pSerial.begin(serial);
}


// Trigger pSerial.update() and check if packet have been missed
void FC_Communication_Base::receiveData()
{
	dataHandledFlag = false;
	
	pSerial.update(); // dataHandledFlag will be changed there if have been packer
	
	if (dataHandledFlag)
		missedPackets=0;
	else
		missedPackets++;
}


// update max amt of lost packets to change comState to false
void FC_Communication_Base::setMaxLostPackets(uint8_t maxLost)
{
	MaxLostPackets = maxLost;
}


// return communication state
bool FC_Communication_Base::comState()
{
	if (missedPackets >= MaxLostPackets)
		return false;
	return true;
}



bool FC_Communication_Base::checkChecksum(const uint8_t* buf, size_t packerSize)
{
	uint8_t checksum = buf[1];
	for (int i=2; i<packerSize; i++)
		checksum ^= buf[i]; // xor'owanie kolejnych bajtow
		
	if (checksum == buf[0])
		return true;
	return false;
}



uint8_t FC_Communication_Base::calcChecksum(const uint8_t* buf, size_t packerSize)
{
	uint8_t checksum = buf[1];
	for (int i=2; i<packerSize; i++)
		checksum ^= buf[i]; // xor'owanie kolejnych bajtow
	
	return checksum;
}

