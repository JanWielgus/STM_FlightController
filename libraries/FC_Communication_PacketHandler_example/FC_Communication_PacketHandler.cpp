// 
// 
// 

#include "FC_Communication_PacketHandler.h"


FC_Communication_PacketHandler::FC_Communication_PacketHandler(Stream* serial, uint8_t bufSize)
	:FC_Communication_Base(serial, bufSize)
{
	
}


/*
	SOME IMPORTANT INFO:
	
		- How to use checksums:
		- buffer[0] in dataPacket is reserved for the checksum value
		- if you are calculating the checksum, store it in the buffer[0]
		  (calculate it after packing data to dataPacket!)
		
		- buffer[1] is reserved for data type (ID) !
		- buffer[0] is not required for checksum if you do not use it
		- buffer[1] is not required for data type (ID) if you use only one data packet type
*/


bool FC_Communication_PacketHandler::receiveAndUnpackData()
{
	bool atLeastOneFlag = false; // at least one packet was received. Needed to return true/false at the end
		
	while (receiveData())
	{
		// Check if this packet is the specific one (TYPE1)
		if (checkReceivedDataPacket(receivedPacketTypes.TYPE1_ID, receivedPacketTypes.TYPE1_SIZE, true))
		{
			// Unpack data by updating proper variables
			// [0] - checksum
			// [1] - ID
			data.received.var1 = dpReceived.buffer[2];
			data.received.liczba.byteArr()[0] = dpReceived.buffer[3];
			data.received.liczba.byteArr()[1] = dpReceived.buffer[4];
			// 4 bytes
			for(int i=0; i<4; i++)
				data.received.zmienna.byteArr()[i] = dpReceived.buffer[5+i];
			data.received.innaLiczba.byteArr()[0] = dpReceived.buffer[9];
			data.received.innaLiczba.byteArr()[1] = dpReceived.buffer[10];
		
			// PACKET SIZE SHOULD BE 11 !!! (10+1=11  counted from zero)
		
			// indicate a successful communication
			atLeastOneFlag = true;
		}
	
	
		// Check if this packet is TYPE2
		/*
		else if (checkReceivedDataPacket(receivedPacketTypes.TYPE2_ID, receivedPacketTypes.TYPE2_SIZE, true))
		{
			// ....
		
		
		
			// indicate a successful communication
			atLeastOneFlag = true;
		}
		*/
	}
	
	
	if (atLeastOneFlag)
		return true;
	return false;
}


void FC_Communication_PacketHandler::packAndSendData(uint8_t packetID)
{
	dpToSend.buffer[1] = packetID;
	
	// TYPE1
	if (packetID == sendPacketTypes.TYPE1_ID)
	{
		// 4 bytes
		for (int i=0; i<4; i++)
			dpToSend.buffer[2+i] = data.toSend.temp.byteArr()[i];
		dpToSend.buffer[6] = data.toSend.zmiennaDoWyslania.byteArr()[0];
		dpToSend.buffer[7] = data.toSend.zmiennaDoWyslania.byteArr()[1];
		dpToSend.buffer[8] = data.toSend.otherVar;
		
		dpToSend.buffer[0] = calcChecksum();
		
		sendData();
	}
	
	// TYPE2
	/*
	else if (packetID == sendPacketTypes.TYPE2_ID)
	{
		// .....
		
		dpToSend.buffer[0] = calcChecksum();
		
		sendData();
	}
	*/
}


bool FC_Communication_PacketHandler::connectionState()
{
	
}


bool FC_Communication_PacketHandler::checkReceivedDataPacket(uint8_t packetID, uint8_t packetSize, bool checkChecksumFlag)
{
	static uint8_t IDpos;
	IDpos = checkChecksumFlag==true ? 1 : 0;
	
	if (dpReceived.buffer[IDpos] == packetID && dpReceived.size == packetSize)
	{
		if (!checkChecksumFlag)
		return true;
		
		//else
		if (checkChecksum())
		return true;
	}
	
	return false;
}


