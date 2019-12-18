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
	
	while (receiveData())
	{
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		// EDIT FOR OTHER PURPOSES FROM HERE
		
		
		
		
		// Check if this packet is the specific one (TYPE1)
		if (checkReceivedDataPacket(receivedPacketTypes.TYPE1_ID, receivedPacketTypes.TYPE1_SIZE, true))
		{
			// voltage 6 cells
			data.received.tilt_TB = dpReceived.buffer[8];
			data.received.tilt_LR = dpReceived.buffer[9];
			data.received.heading.byteArr()[0] = dpReceived.buffer[10];
			data.received.heading.byteArr()[1] = dpReceived.buffer[11];
			data.received.altitude.byteArr()[0] = dpReceived.buffer[12];
			data.received.altitude.byteArr()[1] = dpReceived.buffer[13];
			for (int i=0; i<4; i++)
				data.received.longitude.byteArr()[i] = dpReceived.buffer[14+i];
			for (int i=0; i<4; i++)
				data.received.latitude.byteArr()[i] = dpReceived.buffer[18+i];
			// random value
			data.received.errors1.byte = dpReceived.buffer[23];
			data.received.errors2.byte = dpReceived.buffer[24];
			data.received.bitSwitches1.byte = dpReceived.buffer[25];
			// margin 6x
		}
	
	
		// Check if this packet is TYPE2
		else if (checkReceivedDataPacket(receivedPacketTypes.TYPE2_ID, receivedPacketTypes.TYPE2_SIZE, true))
		{
			// voltage on the lowest cell
			data.received.errors1.byte = dpReceived.buffer[3];
			data.received.errors2.byte = dpReceived.buffer[4];
			data.received.bitSwitches1.byte = dpReceived.buffer[5];
		}
		

		
		
		// TO HERE
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
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
	
	
	
	// TYPE 1 - full background
	if (packetID == sendPacketTypes.TYPE1_ID)
	{
		dpToSend.buffer[2] = data.toSend.pilotDistance.byteArr()[0];
		dpToSend.buffer[3] = data.toSend.pilotDistance.byteArr()[1];
		dpToSend.buffer[4] = data.toSend.pilotDirection.byteArr()[0];
		dpToSend.buffer[5] = data.toSend.pilotDirection.byteArr()[1];
		dpToSend.buffer[6] = data.toSend.flightMode;
		dpToSend.buffer[7] = data.toSend.arming;
		// random value
		dpToSend.buffer[9] = data.toSend.bitSwitches1.byte;
		dpToSend.buffer[10] = data.toSend.bitSwitches2.byte;
		dpToSend.buffer[11] = data.toSend.signalLostScenario;
		
		
		dpToSend.buffer[0] = calcChecksum();
		
		sendData();
	}
	
	// TYPE 2 - basic background
	else if (packetID == sendPacketTypes.TYPE2_ID)
	{
		dpToSend.buffer[2] = data.toSend.flightMode;
		dpToSend.buffer[3] = data.toSend.arming;
		dpToSend.buffer[4] = data.toSend.bitSwitches1.byte;
		dpToSend.buffer[5] = data.toSend.bitSwitches2.byte;
		dpToSend.buffer[6] = data.toSend.signalLostScenario;
		
		
		dpToSend.buffer[0] = calcChecksum();
		
		sendData();
	}
	
	// TYPE 3 - PID parameters
	else if (packetID == sendPacketTypes.TYPE3_ID)
	{
		// There are one place for the PID values and ID of the controller.
		// If need to send values to multiple controllers, send several times and change the controller ID.
		
		
		dpToSend.buffer[2] = data.toSend.PIDcontrollerID;
		// pid values - of the controller which ID is above : 
			// 0 - leveling
			// 1 - yaw
			
		for (int i=0; i<4; i++)
			dpToSend.buffer[3+i] = data.toSend.PIDvalues.P.byteArr()[i];
		for (int i=0; i<4; i++)
			dpToSend.buffer[7+i] = data.toSend.PIDvalues.I.byteArr()[i];
		for (int i=0; i<4; i++)
			dpToSend.buffer[11+i] = data.toSend.PIDvalues.D.byteArr()[i];
		dpToSend.buffer[15] = data.toSend.PIDvalues.I_max;
		
		
		dpToSend.buffer[0] = calcChecksum();
		
		sendData();
	}
	
	// TYPE 4 - steering
	else if (packetID == sendPacketTypes.TYPE4_ID)
	{
		dpToSend.buffer[2] = data.toSend.steer.throttle.byteArr()[0];
		dpToSend.buffer[3] = data.toSend.steer.throttle.byteArr()[1];
		dpToSend.buffer[4] = data.toSend.steer.rotate.byteArr()[0];
		dpToSend.buffer[5] = data.toSend.steer.rotate.byteArr()[1];
		dpToSend.buffer[6] = data.toSend.steer.TB.byteArr()[0];
		dpToSend.buffer[7] = data.toSend.steer.TB.byteArr()[1];
		dpToSend.buffer[8] = data.toSend.steer.LR.byteArr()[0];
		dpToSend.buffer[9] = data.toSend.steer.LR.byteArr()[1];
		
		
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

