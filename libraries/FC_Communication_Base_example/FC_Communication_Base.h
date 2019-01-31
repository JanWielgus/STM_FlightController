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




struct dataPacket
{
	uint8_t* buffer;
	size_t size;
};



class FC_Communication_Base
{
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

	bool checkChecksum(dataPacket& dp); // xor'owanie
	uint8_t calcChecksum(dataPacket& dp);
	

 private:
	uint8_t* receiveBuffer;
	uint8_t* decodeBuffer;
	size_t receiveBufferIndex = 0;
	uint8_t PacketMarker = 0;
	Stream* serial;
};

#endif
