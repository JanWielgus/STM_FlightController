/*
	All data packets that will be sent
*/

#include "CommSendDataPackets.h"

namespace SendData
{
	// add voltage
	int8_t tilt_TB; // top/back
	int8_t tilt_LR; // left/right
	int16Byte heading;
	int16Byte altitude;
	int32Byte longitude;
	int32Byte latitude;
	bitByte errorBits1;
	bitByte errorBits2;
	bitByte bitArray1;
	uint8_t receivingConnectionStability;


	// Packet only instances
	PacketClasses::Basic DP_basic;
	PacketClasses::Full DP_full;
}


