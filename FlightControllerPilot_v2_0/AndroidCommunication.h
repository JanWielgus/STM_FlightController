// AndroidCommunication.h

#ifndef _ANDROIDCOMMUNICATION_h
#define _ANDROIDCOMMUNICATION_h

#include <Arduino.h>

class AndroidCommunication
{
private:
	static const uint8_t MAX_RECEIVE_SIZE = 24;
	static const uint8_t TYPE1_SIZE = 21;

	uint8_t buffor[MAX_RECEIVE_SIZE];

	bool newDataType1ReceivedFlag = false; // true if read() received new data TYPE 1

	uint8_t index = 0; // recent place in the buffor array


public:
	AndroidCommunication();
	void init();
	void read(); // now called in the loop()
	void handleReceivedData();

private:
	int uint8ArrayToInt(uint8_t* arr);
};


#endif

