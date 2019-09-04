// 
// 
// 

#include "FC_MainCommunication.h"


FC_MainCommunication::FC_MainCommunication(Stream* serial, uint8_t bufSize)
	:FC_Communication_Base(serial, bufSize)
{
	conStab = 0;
	
	
	amtOfReceivedDataPackets = (sizeof(receivedPacketTypes) /* / sizeof(uint8_t) */)/2;
	
	if (amtOfReceivedDataPackets > 0)
		receivedDataPacketsList = new bool[amtOfReceivedDataPackets+1]; // +1 to use 1 to TYPE1 and so on...
}


FC_MainCommunication::~FC_MainCommunication()
{
	if (amtOfReceivedDataPackets > 0)
		delete [] receivedDataPacketsList;
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




void FC_MainCommunication::beforeReceiving()
{
	// Need to be reset before receiving. Used to calculate connection stability.
	atLeastOneFlag = false; // at least one packet was received. Needed to return true/false at the end
	
	// reset the list of received data packets
	for (int i=1; i<=amtOfReceivedDataPackets; i++)
		receivedDataPacketsList[i] = false;
}


bool FC_MainCommunication::receiveAndUnpackData()
{
	beforeReceiving();
	
	
	// Receive only one steering data packet (TYPE4_ID)
	// This is because radio module send sometimes two steering data packets and sometimes any
	// If receive only one steering data packet at one function call in next call also will be some steering data (make data continuous)
	while (receiveData())
	{
		bool receivedSteeringPacketFlag = false; // used to end receiving if steering data packet was received
		
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		// EDIT FOR OTHER PURPOSES FROM HERE
		
		
		
		
		// Check if this packet is the specific one (TYPE1)
		if (checkReceivedDataPacket(receivedPacketTypes.TYPE1_ID, receivedPacketTypes.TYPE1_SIZE, true))
		{
			data.received.pilotDistance.byteArr()[0] = dpReceived.buffer[2];
			data.received.pilotDistance.byteArr()[1] = dpReceived.buffer[3];
			data.received.pilotDirection.byteArr()[0] = dpReceived.buffer[4];
			data.received.pilotDirection.byteArr()[1] = dpReceived.buffer[5];
			data.received.flightMode = dpReceived.buffer[6];
			data.received.arming = dpReceived.buffer[7];
			// losowa wartosc
			data.received.bitSwitches1.byte = dpReceived.buffer[9];
			data.received.bitSwitches2.byte = dpReceived.buffer[10];
			data.received.signalLostScenario = dpReceived.buffer[11];
		}
		
		
		// Check if this packet is TYPE2
		else if (checkReceivedDataPacket(receivedPacketTypes.TYPE2_ID, receivedPacketTypes.TYPE2_SIZE, true))
		{
			data.received.flightMode = dpReceived.buffer[2];
			data.received.arming = dpReceived.buffer[3];
			data.received.bitSwitches1.byte = dpReceived.buffer[4];
			data.received.bitSwitches2.byte = dpReceived.buffer[5];
			data.received.signalLostScenario = dpReceived.buffer[6];
		}
		
		
		else if (checkReceivedDataPacket(receivedPacketTypes.TYPE3_ID, receivedPacketTypes.TYPE3_SIZE, true))
		{
			// There are one place for the PID values and ID of the controller.
			// If need to send values to multiple controllers, send several times and change the controller ID.
			
			data.received.PIDcontrollerID = dpReceived.buffer[2];
			// pid values - of the controller which ID is above :
				// 0 - leveling
				// 1 - yaw
			
			for (int i=0; i<4; i++)
				data.received.PIDvalues.P.byteArr()[i] = dpReceived.buffer[i+3];
			for (int i=0; i<4; i++)
				data.received.PIDvalues.I.byteArr()[i] = dpReceived.buffer[i+7];
			for (int i=0; i<4; i++)
				data.received.PIDvalues.D.byteArr()[i] = dpReceived.buffer[i+11];
			data.received.PIDvalues.I_max = dpReceived.buffer[15];
		}
		
		
		else if (checkReceivedDataPacket(receivedPacketTypes.TYPE4_ID, receivedPacketTypes.TYPE4_SIZE, true))
		{
			data.received.steer.throttle.byteArr()[0] = dpReceived.buffer[2];
			data.received.steer.throttle.byteArr()[1] = dpReceived.buffer[3];
			data.received.steer.rotate.byteArr()[0] = dpReceived.buffer[4];
			data.received.steer.rotate.byteArr()[1] = dpReceived.buffer[5];
			data.received.steer.TB.byteArr()[0] = dpReceived.buffer[6];
			data.received.steer.TB.byteArr()[1] = dpReceived.buffer[7];
			data.received.steer.LR.byteArr()[0] = dpReceived.buffer[8];
			data.received.steer.LR.byteArr()[1] = dpReceived.buffer[9];
			
			receivedSteeringPacketFlag = true; // change the flag
		}
		
		
		
		
		// TO HERE
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		
		
		// end if steering data packet was received
		// (prevent from receiving multiple steering data packets at one function call)
		if (receivedSteeringPacketFlag)
			break;
	}
	
	
	// Calculate the connection stability (edit only parameters)
	uint8_t sum = (uint8_t)pastComStatesArr[1] + pastComStatesArr[0] + atLeastOneFlag;
	// TUNE multipliers if needed (depending on the update frequency)
	conStab = sum<conStab ? 0.8*sum + 0.2*conStab  :  0.6*sum + 0.4*conStab; // slower increase than decrease
	// update historic values
	pastComStatesArr[1] = pastComStatesArr[0];
	pastComStatesArr[0] = atLeastOneFlag;
	
	
	if (atLeastOneFlag)
		return true;
	
	return false;
}


void FC_MainCommunication::packAndSendData(uint8_t packetID, uint8_t packetSize)
{
	dpToSend.size = (size_t)packetSize;
	dpToSend.buffer[1] = packetID;
	
	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// CHANGE FOR OTHER PURPOSES FROM HERE
	
	
	
	// TYPE1 - full
	if (packetID == sendPacketTypes.TYPE1_ID)
	{
		// voltage on the 6 cells
		dpToSend.buffer[8] = data.toSend.tilt_TB;
		dpToSend.buffer[9] = data.toSend.tilt_LR;
		dpToSend.buffer[10] = data.toSend.heading.byteArr()[0];
		dpToSend.buffer[11] = data.toSend.heading.byteArr()[1];
		dpToSend.buffer[12] = data.toSend.altitude.byteArr()[0];
		dpToSend.buffer[13] = data.toSend.altitude.byteArr()[1];
		for (int i=0; i<4; i++)
			dpToSend.buffer[i+14] = data.toSend.longitude.byteArr()[i];
		for (int i=0; i<4; i++)
			dpToSend.buffer[i+18] = data.toSend.latitude.byteArr()[i];
		// random value
		dpToSend.buffer[23] = data.toSend.errors1.byte;
		dpToSend.buffer[24] = data.toSend.errors2.byte;
		dpToSend.buffer[25] = data.toSend.bitSwitches1.byte;
		// margin 6x
		
		
		dpToSend.buffer[0] = calcChecksum();
		
		sendData();
	}
	
	// TYPE2 - basic
	else if (packetID == sendPacketTypes.TYPE2_ID)
	{
		// voltage on the lowest cell
		dpToSend.buffer[3] = data.toSend.errors1.byte;
		dpToSend.buffer[4] = data.toSend.errors2.byte;
		dpToSend.buffer[5] = data.toSend.bitSwitches1.byte;
		
		
		dpToSend.buffer[0] = calcChecksum();
		
		sendData();
	}
	
	
	
	// TO HERE
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
}




uint8_t FC_MainCommunication::connectionStability()
{
	// calculated while receiving
	return uint8_t(conStab + 0.5); // average
}


bool FC_MainCommunication::checkReceivedDataPacket(uint8_t packetID, uint8_t packetSize, bool checkChecksumFlag)
{
	uint8_t IDpos;
	IDpos = checkChecksumFlag==true ? 1 : 0;
	
	if (dpReceived.buffer[IDpos] == packetID && dpReceived.size == packetSize)
	{
		// if checkChecksumFlag == false or if checkChecksum==true if checkChecksum()==true
		if ( !checkChecksumFlag || checkChecksum() )
		{
			// check which packet was received
			for (uint8_t i=0; i<amtOfReceivedDataPackets; i++)
			{
				uint8_t* address = ((uint8_t*)(&receivedPacketTypes)) + (i*2); // ID is every 2nd place in the receivedPacketTypes structure
				if ( *address == packetID )
				{
					receivedDataPacketsList[i+1] = true;
					break;
				}
			}
			
			
			atLeastOneFlag = true;
			return true;
		}
	}
	
	return false;
}


// One of the receivedPacketType structure elements can be passed only !!!!!!
bool FC_MainCommunication::wasReceived(const uint8_t& packetID)
{
	uint8_t pos = ((int)(&packetID) - (int)(&receivedPacketTypes))/2 + 1; // number of checked type (TYPE1 = 1, TYPE2 = 2,... ONLY IF IN ORDER !!! )
	
	if (pos <= amtOfReceivedDataPackets)
		return receivedDataPacketsList[pos];
	return false;
}

