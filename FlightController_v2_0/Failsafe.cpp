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

	if (config::failsafeSettings.SuppressMotorsAndDisarmWhenLostSignal)
		suppressMotorsAndDisarmWhenLostSignal();
}


void Failsafe::cutoffMotorsIfExceededAngle()
{
	//Cut-off all motors if the angle is too high
	if (Storage::reading.angle.x > config::failsafeSettings.CutOffAngle ||
		Storage::reading.angle.x < -config::failsafeSettings.CutOffAngle ||
		Storage::reading.angle.y > config::failsafeSettings.CutOffAngle ||
		Storage::reading.angle.y < -config::failsafeSettings.CutOffAngle)
	Storage::motors.setMotorState(false);
}


void Failsafe::suppressMotorsAndDisarmWhenLostSignal()
{
	if (Storage::comm.getConnectionStability() < config::failsafeSettings.ConnectionStabilityBorder)
	{
		if (ReceiveData::throttle > 20)
		{
			// value *= 0.5
			ReceiveData::throttle = ReceiveData::throttle * config::failsafeSettings.suppressMultiplier;
			ReceiveData::rot_stick = ReceiveData::rot_stick * config::failsafeSettings.suppressMultiplier;
			ReceiveData::TB_stick = ReceiveData::TB_stick * config::failsafeSettings.suppressMultiplier;
			ReceiveData::LR_stick = ReceiveData::LR_stick * config::failsafeSettings.suppressMultiplier;
		}
		else
		{
			Storage::motors.setMotorState(false);
			digitalWrite(config::pin.blueDiode, LOW); // disable diode to show that unarmed
			// Do not change flight mode, because when signal will be back, motors will spin again
			// Because previous flight mode will be restored by pilot data packet
		}
	}
}

