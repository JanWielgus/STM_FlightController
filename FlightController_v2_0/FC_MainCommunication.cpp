// 
// 
// 

#include "FC_MainCommunication.h"


FC_MainCommunication::FC_MainCommunication(Stream* serial, uint8_t bufSize)
	:FC_Communication_Base(serial, bufSize)
{
	conStab = 0;
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




bool FC_MainCommunication::receiveAndUnpackData()
{
	atLeastOneFlag = false; // at least one packet was received. Needed to return true/false at the end
		
	while (receiveData())
	{
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		// CHANGE FOR OTHER PURPOSES FROM HERE
		
		
		
		
		// Check if this packet is the specific one (TYPE1)
		if (checkReceivedDataPacket(receivedPacketTypes.TYPE1_ID, receivedPacketTypes.TYPE1_SIZE, true))
		{
			data.received.steer.throttle.byteArr()[0] = dpReceived.buffer[2];
			data.received.steer.throttle.byteArr()[1] = dpReceived.buffer[3];
			data.received.steer.rotate.byteArr()[0] = dpReceived.buffer[4];
			data.received.steer.rotate.byteArr()[1] = dpReceived.buffer[5];
			data.received.steer.TB.byteArr()[0] = dpReceived.buffer[6];
			data.received.steer.TB.byteArr()[1] = dpReceived.buffer[7];
			data.received.steer.LR.byteArr()[0] = dpReceived.buffer[8];
			data.received.steer.LR.byteArr()[1] = dpReceived.buffer[9];
			data.received.pilotDistance.byteArr()[0] = dpReceived.buffer[10];
			data.received.pilotDistance.byteArr()[1] = dpReceived.buffer[11];
			data.received.pilotDirection.byteArr()[0] = dpReceived.buffer[12];
			data.received.pilotDirection.byteArr()[1] = dpReceived.buffer[13];
			data.received.flightMode = dpReceived.buffer[14];
			data.received.arming = dpReceived.buffer[15];
			// losowa wartosc
			data.received.bitSwitches1.byte = dpReceived.buffer[17];
			data.received.bitSwitches2.byte = dpReceived.buffer[18];
			data.received.signalLostScenario = dpReceived.buffer[19];
			// margin 6x
		}
	
	
		// Check if this packet is TYPE2
		else if (checkReceivedDataPacket(receivedPacketTypes.TYPE2_ID, receivedPacketTypes.TYPE2_SIZE, true))
		{
			data.received.steer.throttle.byteArr()[0] = dpReceived.buffer[2];
			data.received.steer.throttle.byteArr()[1] = dpReceived.buffer[3];
			data.received.steer.rotate.byteArr()[0] = dpReceived.buffer[4];
			data.received.steer.rotate.byteArr()[1] = dpReceived.buffer[5];
			data.received.steer.TB.byteArr()[0] = dpReceived.buffer[6];
			data.received.steer.TB.byteArr()[1] = dpReceived.buffer[7];
			data.received.steer.LR.byteArr()[0] = dpReceived.buffer[8];
			data.received.steer.LR.byteArr()[1] = dpReceived.buffer[9];
			data.received.flightMode = dpReceived.buffer[10];
			data.received.arming = dpReceived.buffer[11];
			data.received.bitSwitches1.byte = dpReceived.buffer[12];
			data.received.bitSwitches2.byte = dpReceived.buffer[13];
			data.received.signalLostScenario = dpReceived.buffer[14];
		}
		
		
		else if (checkReceivedDataPacket(receivedPacketTypes.TYPE3_ID, receivedPacketTypes.TYPE3_SIZE, true))
		{
			// Leveling PID parameters
			for (int i=0; i<4; i++)
				data.received.levelingPID.P.byteArr()[i] = dpReceived.buffer[i+2];
			for (int i=0; i<4; i++)
				data.received.levelingPID.I.byteArr()[i] = dpReceived.buffer[i+6];
			for (int i=0; i<4; i++)
				data.received.levelingPID.D.byteArr()[i] = dpReceived.buffer[i+10];
			data.received.levelingPID.I_max = dpReceived.buffer[14];
			
			// Yaw control PID parameters
			for (int i=0; i<4; i++)
				data.received.yawPID.P.byteArr()[i] = dpReceived.buffer[i+15];
			for (int i=0; i<4; i++)
				data.received.yawPID.I.byteArr()[i] = dpReceived.buffer[i+19];
			for (int i=0; i<4; i++)
				data.received.yawPID.D.byteArr()[i] = dpReceived.buffer[i+23];
			data.received.yawPID.I_max = dpReceived.buffer[27];
			
			// altHold PID parameters
			for (int i=0; i<4; i++)
				data.received.altHoldPID.P.byteArr()[i] = dpReceived.buffer[i+28];
			for (int i=0; i<4; i++)
				data.received.altHoldPID.I.byteArr()[i] = dpReceived.buffer[i+32];
			for (int i=0; i<4; i++)
				data.received.altHoldPID.D.byteArr()[i] = dpReceived.buffer[i+36];
			data.received.altHoldPID.I_max = dpReceived.buffer[40];
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
		if (!checkChecksumFlag)
		{
			atLeastOneFlag = true;
			return true;
		}
		
		//else
		if (checkChecksum())
		{
			atLeastOneFlag = true;
			return true;
		}
	}
	
	return false;
}

