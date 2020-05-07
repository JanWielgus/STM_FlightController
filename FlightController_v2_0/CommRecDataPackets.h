// CommRecDataPackets.h
/*
	All data packets that could be received
*/

#ifndef _COMMRECDATAPACKETS_h
#define _COMMRECDATAPACKETS_h

#include "arduino.h"
#include <DataPacketBase.h>
#include <FC_CustomDataTypes.h>
#include "SharedDataTypes.h"

namespace ReceiveData
{
	// steering
	extern uint16Byte throttle;
	extern int16Byte rot_stick;
	extern int16Byte TB_stick;
	extern int16Byte LR_stick;

	extern uint16Byte pilotDistance;
	extern uint16Byte pilotDirection;
	extern uint8_t flightMode;
	extern bitByte bitArray1;
	extern bitByte bitArray2;
	extern uint8_t signalLostScenario;

	extern uint8_t tunedControllerID; // which controller is tuned
	extern pidCommPack tunedPID_values; // PID values of that controller


	
	namespace PacketClasses
	{
		class Steering : public DataPacketBase
		{
		public:
			Steering()
				: DataPacketBase(0)
			{
				addByteType(throttle);
				addByteType(rot_stick);
				addByteType(TB_stick);
				addByteType(LR_stick);
			}
		};


		class FullBackground : public DataPacketBase
		{
		public:
			FullBackground()
				: DataPacketBase(1)
			{
				// all byte types
				addByteType(pilotDistance);
				addByteType(pilotDirection);
				addByteType(bitArray1);
				addByteType(bitArray2);

				// other
				add_uint8_t(flightMode);
				add_uint8_t(signalLostScenario);
			}
		};


		class BasicBackground : public DataPacketBase
		{
		public:
			BasicBackground()
				: DataPacketBase(2)
			{
				// all byte types
				addByteType(bitArray1);
				addByteType(bitArray2);

				// other
				add_uint8_t(flightMode);
				add_uint8_t(signalLostScenario);
			}
		};


		class PID_params : public DataPacketBase
		{
		public:
			PID_params()
				: DataPacketBase(3)
			{
				add_uint8_t(tunedControllerID);
				addByteType(tunedPID_values.P);
				addByteType(tunedPID_values.I);
				addByteType(tunedPID_values.I_max);
				addByteType(tunedPID_values.D);
			}
		};
	}


	// Packet only instances
	extern PacketClasses::Steering DP_steering;
	extern PacketClasses::BasicBackground DP_basicBackground;
	extern PacketClasses::FullBackground DP_fullBackground;
	extern PacketClasses::PID_params DP_PID_params;
}


#endif

