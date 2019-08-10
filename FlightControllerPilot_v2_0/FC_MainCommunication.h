// FC_MainCommunication.h

#ifndef _FC_MAINCOMMUNICATION_h
#define _FC_MAINCOMMUNICATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <FC_Communication_Base.h>
#include <FC_CustomDataTypes.h>

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
	
	~FC_MainCommunication();
	
	bool receiveAndUnpackData();                  // receive proper data packet/packets, returns true if at least one data packet was received
	void packAndSendData(uint8_t packetID, uint8_t packetSize);       // pack data to the data packet and send it
	uint8_t connectionStability();                // 0-no connection, 1-minor com.  <---> 3-stable com
	bool wasReceived(const uint8_t& packerID);    // check whether packetID was received in this receiving (used after receiveAndUnpackData() )
	
	
 private:
	// check if data is a certain packet with ID and SIZE
	bool checkReceivedDataPacket(uint8_t packetID, uint8_t packetSize, bool checkChecksumFlag = false);
		// packetID - check if this data packet is a specific data packet with that ID
		// packetSize - check if this data packet has the same size as should have
		// checkChecksumFlag - if there is a need to check checksum (if not, IDpos is default 0 -> first in buffer)
		// (in code) IDpos - position of the packetID in buffer (auto: checkChecksum - 1, else - 0)
	
	void beforeReceiving(); // do things before receiving
		
		
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
		
	} receivedPacketTypes;
	
	struct
	{
		// full background
		const uint8_t TYPE1_ID = 0x01;
		const uint8_t TYPE1_SIZE = 12;
		// basic background
		const uint8_t TYPE2_ID = 0x02;
		const uint8_t TYPE2_SIZE = 7;
		// pid params
		const uint8_t TYPE3_ID = 0x03;
		const uint8_t TYPE3_SIZE = 16;
		// steering
		const uint8_t TYPE4_ID = 0x04;
		const uint8_t TYPE4_SIZE = 10;
	} sendPacketTypes;
	
	
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
		// voltage
		int8_t tilt_TB; // top/back
		int8_t tilt_LR; // left/right
		int16Byte heading;
		int16Byte altitude;
		int32Byte longitude;
		int32Byte latitude;
		// random value
		bitByte errors1;
		bitByte errors2;
		bitByte bitSwitches1;
		
		
		// TYPE2 - basic
		// voltage on the lowest cell
		// errors 1 & 2 (up)
		// bitSwitch 1 (up)
	};
	
	struct toSendDataType
	{
		// all data to send
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
		// random value
		bitByte bitSwitches1;
		bitByte bitSwitches2;
		uint8_t signalLostScenario;
		
		// TYPE2 - basic
		// just part of TYPE1
		
		// TYPE3 - PID params
		uint8_t PIDcontrollerID; // which controller is tuned
		comPidPack PIDvalues; // PID values of that controller
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
	bool atLeastOneFlag; // used only in the receiveAndUnpackData() method
	
	
	uint8_t amtOfReceivedDataPackets; // automatically calculated amt of different received data packets
	bool* receivedDataPacketsList; // true - data packet [x] was received when called receiveAndUnpack()
		// used to check if specific data packet was received in one receive method call
		// eg. TYPE1 was received if receivedDataPacketList[1] == true
	
	
 public:
	// This is to access data from outside the class. 
	toSendDataType& toSend = data.toSend;                // put data here (outside the class)
	const receivedDataType& received = data.received;    // read-only, read data from here
};


#endif

