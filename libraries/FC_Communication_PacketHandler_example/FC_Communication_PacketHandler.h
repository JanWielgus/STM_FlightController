// FC_Communication_PacketHandler.h

#ifndef _FC_COMMUNICATION_PACKETHANDLER_h
#define _FC_COMMUNICATION_PACKETHANDLER_h

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



class FC_Communication_PacketHandler : public FC_Communication_Base
{
 public:
	FC_Communication_PacketHandler(Stream* serial, uint8_t bufSize);
	
	bool receiveAndUnpackData();                  // receive proper data packet/packets, returns true if at least one data packet was received
	void packAndSendData(uint8_t packetID);       // pack data to the data packet and send it
	bool connectionState();                       // returns true if data are being properly unpacked
	
	// check if data is a certain packet with ID and SIZE
	bool checkReceivedDataPacket(uint8_t packetID, uint8_t packetSize, bool checkChecksumFlag = false, uint8_t IDpos = 0);
		// packetID - check if this data packet is a specific data packet with that ID
		// packetSize - check if this data packet has the same size as should have
		// checkChecksumFlag - if there is a need to check checksum (if not, IDpos is default 0 -> first in buffer)
		// IDpos - position of the packetID in buffer (if use checksum -> 1, if not -> 0)
	
	
 private:

	struct receivedDataType
	{
		// all received data
		
		//TYPE1
		uint8_t var1;
		int16_t liczba;
		float zmienna;
		int16_t innaLiczba;
		
		//TYPE2
		//...
	};
	
	struct toSendDataType
	{
		// all data to send
		
		//TYPE1
		float temp;
		int16_t zmiennaDoWyslania;
		uint8_t otherVar;
		
		//TYPE2
		//...
	};
	
	// data packet types and sizes
	//TYPE1
	struct
	{
		const uint8_t TYPE1_ID = 0x01; // Unique ID
		const uint8_t TYPE1_SIZE = 8;  // How many uint8_t's is required to send all TYPE1 data packet
		// Other types (pairs ID and SIZE)
	} sendPacketTypes;
	
	struct
	{
		const uint8_t TYPE1_ID = 0x01;
		const uint8_t TYPE1_SIZE = 10;  // How many uint8_t's is required to receive all TYPE1 data packet
		// Other types (pairs ID and SIZE)
	} receivedPacketTypes;
	
	
	// There data is created (access from inside the class)
	struct
	{
		receivedDataType received;
		toSendDataType toSend;
	} data;
	
	
 public:
	// This is to access data from outside the class. 
	toSendDataType& toSend = data.toSend;                // put data here (outside the class)
	const receivedDataType& received = data.received;    // read-only, read data from here
};


#endif


