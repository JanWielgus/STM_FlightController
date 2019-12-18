#include "AndroidCommunication.h"
#include "Storage.h"


AndroidCommunication::AndroidCommunication()
{
	// do nothing
}


void AndroidCommunication::init()
{
	bluetoothSoftwareSerial.begin(9600);
}


// When need to receive 21 bytes, it could be in smaller parts but should be the whole data
void AndroidCommunication::read()
{
	if (bluetoothSoftwareSerial.available())
	{
		while (bluetoothSoftwareSerial.available() > 0)
		{
			// If buffer overflow, exit function
			if (index >= MAX_RECEIVE_SIZE)
			{
				newDataType1ReceivedFlag = false;
				return;
			}

			buffor[index++] = bluetoothSoftwareSerial.read();
		}

		// Check if data is type 1
		if (index == TYPE1_SIZE && buffor[0] == 123)
			newDataType1ReceivedFlag = true;
	}
}


// Merge ints divided in the Android app to 4 x uint8
int AndroidCommunication::uint8ArrayToInt(uint8_t* arr)
{
	int result = arr[0] << 24 | (arr[1] & 0xFF) << 16 | (arr[2] & 0xFF) << 8 | (arr[3] & 0xFF);
	return result;
}


void AndroidCommunication::handleReceivedData()
{
	// reset the index
	index = 0;

	// read data
	if (newDataType1ReceivedFlag)
	{
		newDataType1ReceivedFlag = false;

		androidData.controllerID = uint8ArrayToInt(buffor + 1);
		androidData.PID_P = (float)uint8ArrayToInt(buffor + 5) / 100.0;
		androidData.PID_I = (float)uint8ArrayToInt(buffor + 9) / 100.0;
		androidData.PID_Imax = (uint8_t)uint8ArrayToInt(buffor + 13);
		androidData.PID_D = (float)uint8ArrayToInt(buffor + 17) / 100.0;

		flags.btNeedToUpdatePID = true;
	}
}

