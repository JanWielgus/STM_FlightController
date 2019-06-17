// 
// 
// 

#include "FC_Communication_PacketHandler.h"


FC_Communication_PacketHandler::FC_Communication_PacketHandler(Stream* serial, uint8_t bufSize)
	:FC_Communication_Base(serial, bufSize)
{
	
}


bool FC_Communication_PacketHandler::receiveAndUnpackData()
{
	// Stop if any data packet was received
	if (!receiveData())
		return false;
	
	
	
	
	
	
	
	
	
	return true;
}


void FC_Communication_PacketHandler::packAndSendData(uint8_t packetID)
{
	
}


bool FC_Communication_PacketHandler::connectionState()
{
	
}


