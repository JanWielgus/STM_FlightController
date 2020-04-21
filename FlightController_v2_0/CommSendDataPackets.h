// CommSendDataPackets.h
/*
	All data packets that will be sent
*/

#ifndef _COMMSENDDATAPACKETS_h
#define _COMMSENDDATAPACKETS_h

#include "arduino.h"
#include <DataPacketBase.h>
#include <FC_CustomDataTypes.h>

namespace SendData
{
	extern int8_t tilt_TB; // top/back
	extern int8_t tilt_LR; // left/right
	extern int16Byte heading;
	extern int16Byte altitude;
	extern int32Byte longitude;
	extern int32Byte latitude;
	extern bitByte errorBits1;
	extern bitByte errorBits2;
	extern bitByte bitArray1;



	namespace PacketClasses
	{
		class Full : public DataPacketBase
		{
		public:
			Full()
				: DataPacketBase(0)
			{
				// add voltage

				// byte types
				addByteType(heading);
				addByteType(altitude);
				addByteType(longitude);
				addByteType(latitude);
				addByteType(errorBits1);
				addByteType(errorBits2);
				addByteType(bitArray1);

				// other types
				add_int8_t(tilt_TB);
				add_int8_t(tilt_LR);
			}
		};


		class Basic : public DataPacketBase
		{
		public:
			Basic()
				: DataPacketBase(1)
			{
				// byte types
				addByteType(errorBits1);
				addByteType(errorBits2);
				addByteType(bitArray1);
			}
		};
	}


	// Packet only instances
	extern PacketClasses::Basic DP_basic;
	extern PacketClasses::Full DP_full;
}


#endif

