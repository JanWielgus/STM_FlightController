/*
 * BluetoothDevice.h
 *
 * Created: 08/08/2019 19:21:05
 *  Author: Jan Wielgus
 */ 


#ifndef BLUETOOTHDEVICE_H_
#define BLUETOOTHDEVICE_H_

#include "Storage.h"



namespace btDevice
{
	const uint8_t MAX_RECEIVE_SIZE = 24;
	const uint8_t TYPE1_SIZE = 21;
		
	uint8_t buffor[MAX_RECEIVE_SIZE];
	
	bool newDataType1ReceivedFlag = false; // true if read() received new data TYPE 1
	
	uint8_t index = 0; // recent place in the buffor array
	
	
	void init()
	{
		bluetoothSoftwareSerial.begin(9600);
	}
	
	
	// When need to receive 21 bytes, it could be in smaller parts but should be the whole data
	void read()
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
	
	
	
	// Merge ints divided in the Android app to 4xuint8
	int uint8ArrayToInt(uint8_t* arr)
	{
		int result = arr[0] << 24 | (arr[1] & 0xFF) << 16 | (arr[2] & 0xFF) << 8 | (arr[3] & 0xFF);
		return result;
	}
	
	
	
	void handleReceivedData()
	{
		// reset the index
		index = 0;
		
		// read data
		if (newDataType1ReceivedFlag)
		{
			newDataType1ReceivedFlag = false;
			
			btControllerID = uint8ArrayToInt(buffor + 1);
			btPID_P = (float)uint8ArrayToInt(buffor + 5) / 100.0;
			btPID_I = (float)uint8ArrayToInt(buffor + 9) / 100.0;
			btPID_Imax = (uint8_t)uint8ArrayToInt(buffor + 13);
			btPID_D = (float)uint8ArrayToInt(buffor + 17) / 100.0;
			
			btNeedToUpdatePIDFlag = true;
		}
	}
	
	
	
} // end of the namespace



#endif /* BLUETOOTHDEVICE_H_ */