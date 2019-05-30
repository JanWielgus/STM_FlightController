// 
// 
// 

#include "FC_Communication_PacketHandler.h"


FC_Communication_PacketHandler::FC_Communication_PacketHandler(Stream* serial, uint8_t bufSize)
	:FC_Communication_Base(serial, bufSize)
{
	
}



void FC_Communication_PacketHandler::getReceivedData(receivedGardenData& dataContainer)
{
	dataContainer = allData;
}



