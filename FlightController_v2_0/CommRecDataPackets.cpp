/*
	All data packets that could be received
*/

#include "CommRecDataPackets.h"

namespace ReceiveData
{
	// steering
	uint16Byte throttle;
	int16Byte rot_stick;
	int16Byte TB_stick;
	int16Byte LR_stick;

	uint16Byte pilotDistance;
	uint16Byte pilotDirection;
	uint8_t flightMode;
	uint8_t arming;
	bitByte bitArray1;
	bitByte bitArray2;
	uint8_t signalLostScenario;

	uint8_t PIDcontrollerID; // which controller is tuned
	pidPack PIDvalues; // PID values of that controller
}


