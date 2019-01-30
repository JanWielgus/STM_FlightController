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


const size_t BufferSize = 100; // MAX: 256



class FC_Communication_Base
{
 public:
	typedef void (*PacketHandlerFunction)(const uint8_t* buffer, size_t size);
	struct dataPacket
	{
		uint8_t* buffer;
		size_t size;
	};

	FC_Communication_Base(Stream* serial);
	
	void sendData(dataPacket packet);
	bool receiveData(dataPacket* packet); // return false if there are no data
	//bool isAvailable(); // receiveData() return true if was available and false if not
	void setMaxLostPackets(uint8_t maxLost);
	

 protected:
	bool checkChecksum(const uint8_t* buffer, size_t packerSize); // xor'owanie
	uint8_t calcChecksum(const uint8_t* buffer, size_t packerSize);
	

 private:
	uint8_t receiveBuffer[BufferSize];
	uint8_t decodeBuffer[BufferSize];
	size_t receiveBufferIndex = 0;
	uint8_t PacketMarker = 0;
	Stream* serial;
};

#endif

