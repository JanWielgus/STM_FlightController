// FC_Communication_Base.h
//
// Author: Jan Wielgus
// Date: 29.01.2019
//

#ifndef _FC_COMMUNICATION_BASE_h
#define _FC_COMMUNICATION_BASE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Arduino.h>
#include "Encoding/COBS.h"
//#include "Encoding/SLIP.h" // alternative

/*
typedef uint8 uint8_t;
typedef uint16 uint16_t;
typedef uint8 size_t;
*/



class FC_Communication_Base
{
 private:
	struct dataPacket
	{
		uint8_t* buffer;
		size_t size;
	};
	
	
 public:
	const size_t BufferSize; // MAX: 256
	//typedef void (*PacketHandlerFunction)(const uint8_t* buffer, size_t size);

	FC_Communication_Base(Stream* serial, uint8_t bufSize=255); // serial, packetToPrepare - packet used outside to send data (there memory is allocated), bufSize - max buffer size
	~FC_Communication_Base();
	
	void sendData(); // data to send packet before to dpToSend
	bool receiveData(); // return false if there are no data
	//bool isAvailable(); // receiveData() return true if was available and false if not
	
	// dataPackets to transfer data between class and outside
	dataPacket dpToSend; // data packet used to send data (filled outside)
	dataPacket dpReceived; // data packet with received data (used outside to unpack data)

	/*
		How to use checksums:
		- buffer[0] in dataPacket is reserved for the checksum value
		- if you are calculating the checksum, store it in the buffer[0]
		  (calculate it after packing data to dataPacket!)
	*/
	bool checkChecksum(); // xor'owanie
	uint8_t calcChecksum();
	
	// check if data is a certain packet with ID and SIZE
	bool checkReceivedDataPacket(uint8_t packetID, uint8_t packetSize, bool checkChecksumFlag = false, uint8_t IDpos = 0);
		// packetID - check if this data packet is a specific data packet with that ID
		// packetSize - check if this data packet has the same size as should have
		// checkChecksumFlag - if there is a need to check checksum (if not, IDpos is default 0 -> first in buffer)
		// IDpos - position of the packetID in buffer (if use checksum -> 1, if not -> 0)
	

 private:
	uint8_t* receiveBuffer;
	uint8_t* decodeBuffer;
	size_t receiveBufferIndex = 0;
	uint8_t PacketMarker = 0;
	Stream* serial;
};

#endif
