// 
// 
// 

#include "Failsafe.h"
#include "Storage.h"
#include "config.h"


void Failsafe::execute()
{
	if (config::failsafeSettings.CutOffMotorsIfAngleExceeded)
		cutoffMotorsIfExceededAngle();

	if (config::failsafeSettings.DisarmMotorsWhenLostSignal)
		DisarmMotorsWhenLostSignal();
}


void Failsafe::cutoffMotorsIfExceededAngle()
{
	//Cut-off all motors if the angle is too high
	if (abs(Storage::reading.pitch) > config::failsafeSettings.CutOffAngle ||
		abs(Storage::reading.roll) > config::failsafeSettings.CutOffAngle)
		cutOffMotors();
}


void Failsafe::DisarmMotorsWhenLostSignal()
{
	if (Storage::comm.getConnectionStability() < config::failsafeSettings.ConnectionStabilityBorder)
		cutOffMotors();
}




void Failsafe::cutOffMotors()
{
	Storage::motors.setMotorState(false);
	digitalWrite(config::pin.blueDiode, LOW); // disable diode to show that unarmed
	// Do not change flight mode, because when signal will be back, motors will spin again
	// Because previous flight mode will be restored by pilot data packet
}

