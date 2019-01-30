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

#include <PacketSerial.h>



class FC_Communication_Base
{
 public:
	FC_Communication_Base(Stream* serial);
	virtual void sendData(uint8_t pcktType) = 0;
	void receiveData();
	bool comState();
	void setMaxLostPackets(uint8_t maxLost);
	

 protected:
	virtual void handleReceivedData(const uint8_t* buf, size_t size) = 0; // prev odbierzPriv()
	PacketSerial pSerial;
	Stream* comSerial;
	bool checkChecksum(const uint8_t* buf, size_t packerSize); // xor'owanie
	uint8_t calcChecksum(const uint8_t* buf, size_t packerSize);
	

 private:
	uint8_t MaxLostPackets=0;
	bool dataHandledFlag; // to check if it was changed on true in handleReceivedData (if any data packet have been decoded)
	uint16_t missedPackets = 0;
	
};

#endif

