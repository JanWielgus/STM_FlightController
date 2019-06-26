// FC_MainCommunication.h

#ifndef _FC_MAINCOMMUNICATION_h
#define _FC_MAINCOMMUNICATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <FC_Communication_Base.h>

/*
	HOW TO USE:
	- In the class use data structure to access the data. This should be the only place, where whole
	  sent and received data are stored.
	- Outside the class use toSend and received public references to the data (data to send place
	  in toSend structure reference before sending, in the received structure reference)
	- All variables to send have to be created in toSendDataType and only there.
	- All variables to receive have to be created in receivedDataType and only there.
*/



class FC_MainCommunication : private FC_Communication_Base
{
 public:
	FC_MainCommunication(Stream* serial, uint8_t bufSize);
		// bufSize - the size of the largest data packet + ~2 for safety
	
	bool receiveAndUnpackData();                  // receive proper data packet/packets, returns true if at least one data packet was received
	void packAndSendData(uint8_t packetID);       // pack data to the data packet and send it
	uint8_t connectionStability();                // 0-no connection, 1-minor com.  <---> 3-stable com
	
	
 private:
	// check if data is a certain packet with ID and SIZE
	bool checkReceivedDataPacket(uint8_t packetID, uint8_t packetSize, bool checkChecksumFlag = false);
		// packetID - check if this data packet is a specific data packet with that ID
		// packetSize - check if this data packet has the same size as should have
		// checkChecksumFlag - if there is a need to check checksum (if not, IDpos is default 0 -> first in buffer)
		// (in code) IDpos - position of the packetID in buffer (auto: checkChecksum - 1, else - 0)
		
		
 public:
	
	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// CHANGE FOR OTHER PURPOSES FROM HERE
	
	
	
	
	// data packet types and sizes
	//TYPE1
	struct
	{
		// full
		const uint8_t TYPE1_ID = 0x01;
		const uint8_t TYPE1_SIZE = 32;
		// basic
		const uint8_t TYPE2_ID = 0x02;
		const uint8_t TYPE2_SIZE = 6;
		
	} sendPacketTypes;
	
	struct
	{
		// full
		const uint8_t TYPE1_ID = 0x01;
		const uint8_t TYPE1_SIZE = 26;
		// basic
		const uint8_t TYPE2_ID = 0x02;
		const uint8_t TYPE2_SIZE = 15;
		// pid params
		const uint8_t TYPE3_ID = 0x03;
		const uint8_t TYPE3_SIZE = 41;
	} receivedPacketTypes;
	
	
 private:
	struct comPidPack
	{
		floatByte P;
		floatByte I;
		floatByte D;
		uint8_t I_max;
	};
	
	struct receivedDataType
	{
		// all received data
		// use only variables that can be divided into uint8_t's
		
		// TYPE1 - full
		struct
		{
			uint16Byte throttle;
			int16Byte rotate;
			int16Byte TB;
			int16Byte LR;
		} steer;
		uint16Byte pilotDistance;
		uint16Byte pilotDirection;
		uint8_t flightMode;
		uint8_t arming;
		// losowa wartosc
		bitByte bitSwitches1;
		bitByte bitSwitches2;
		uint8_t signalLostScenario;
		
		// TYPE2 - basic
		// just part of TYPE1
		
		// TYPE3 - PID params
		comPidPack levelingPID;
		comPidPack yawPID;
		comPidPack altHoldPID;
	};
	
	struct toSendDataType
	{
		// all data to send
		// use only variables that can be divided into uint8_t's
		
		
		// TYPE1 - full
		//napiecie
		int8_t tilt_TB; // top/back
		int8_t tilt_LR; // left/right
		int16Byte heading;
		int16Byte altitude;
		int32Byte longitude;
		int32Byte latitude;
		// wartosc losowa
		bitByte errors1;
		bitByte errors2;
		bitByte bitSwitches1;
		// zapas 6x
		
		
		// TYPE2 - basic
		// napiecie najnizszej celi
		// errors 1 & 2 /\
		// bitSwitch 1 /\
	};
	
	
	
	// TO HERE
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	
	
	// There data is created (access from inside the class)
	struct
	{
		receivedDataType received;
		toSendDataType toSend;
	} data;
	
	bool pastComStatesArr[2] = {}; // 2 because you need 2 past values and one present to have 3 max (read conStab() description), 0-newer, 1-older
	float conStab; // connectionStability method return this value. Calculated in the receive function
	
	
 public:
	// This is to access data from outside the class. 
	toSendDataType& toSend = data.toSend;                // put data here (outside the class)
	const receivedDataType& received = data.received;    // read-only, read data from here
};


#endif

