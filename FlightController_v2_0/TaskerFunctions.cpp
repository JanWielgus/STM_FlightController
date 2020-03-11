#include "TaskerFunctions.h"
#include "Storage.h"
#include "config.h"

/*
using Storage::levelXpid;
using Storage::levelYpid;
using Storage::yawPID;
using Storage::altHoldPID;
using Storage::motors;
using Storage::com;
using Storage::flags;*/

using namespace Storage;



void addTaskerFunctionsToTasker()
{
	using namespace TaskerFunction;
	using Storage::tasker;


	// Maximum tasker tasks amt is set in config::MaxAmtOfTaskerTasks


	// General
	tasker.addTask(new UpdateControlDiode, 100000L, 2);				// 1Hz (tested duration)
	tasker.addTask(new CheckCalibrations, 700000L, 7);				// 1.4Hz

	// Steering
	tasker.addTask(new ReadXY_angles, config::MainInterval, 639);	// 250Hz (tested duration)
	tasker.addTask(new ReadCompass, 13340L, 492);					// 75Hz  (tested duration)
	tasker.addTask(&Storage::virtualPilot, config::MainInterval, 0);// 250Hz

	// !! tasker.addTask(new Stabilize, 4000L, 31);						// 250Hz (duration tested only with leveling)


	// Communication
	tasker.addTask(new UpdateSending, 22000L, 1);					// ~45Hz
	tasker.addTask(new UpdateReceiving, 7142L, 1);					// 140Hz (! UPDATE the body if frequency chanded !!!)
}



namespace TaskerFunction
{
	void UpdateControlDiode::execute()
	{
		digitalWrite(config::pin.LedBuiltIn, ledState);
		ledState = !ledState;
	}


	void CheckCalibrations::execute()
	{
		//...
		// accelerometer calibration
		// gyroscope calibration
		// compass calibration

		// remember about reseting and setting proper variables after calibration,
		// eg. set fused angle inside the MPU class after accelermeter calibration (may have to write a proper method)
		// eg. set initial Z axis in the MPU after calibrating the compass
		// ...
	}


	void ReadXY_angles::execute()
	{
		mpu.read6AxisMotion();
		angle = mpu.getFusedXYAngles();
		//heading = mpu.getZAngle(compass.getHeading()); // Temporary (something bad is happening with the compass)
		heading = mpu.getZAngle();
	}


	void ReadCompass::execute()
	{
		compass.readCompassData(angle.x, angle.y);
	}



	//// !!!!!
	//// THIS PART HAVE TO BE REMOVED (ONLY VIRTUAL PILOT USE FLIGHT MODES)
	//void Stabilize::execute()
	//{
	//	// Cut-off all motors if the angle is too high
	//	using namespace config;
	//	if (angle.x > CutOffAngle || angle.x < -CutOffAngle ||
	//		angle.y > CutOffAngle || angle.y < -CutOffAngle)
	//		motors.setMotorState(false);





	//	// Extrapolate sticks
	//	// Extrapolate TB and LR stick values
	//	if (flags.needToExtrapolateStickVlaues)
	//	{
	//		extrapolatedTBstick += (float(com.received.steer.TB - previousTBvalue) * 0.2);
	//		extrapolatedLRstick += (float(com.received.steer.LR - previousLRvalue) * 0.2);
	//	}
	//	else
	//	{
	//		extrapolatedTBstick = (float)com.received.steer.TB;
	//		extrapolatedLRstick = (float)com.received.steer.LR;
	//	}
	//	// next run should extrapolate sticks values unless communication will reset this flag
	//	extrapolatedTBstick = tbFilter.updateFilter(extrapolatedTBstick);
	//	extrapolatedLRstick = lrFilter.updateFilter(extrapolatedLRstick);
	//	flags.needToExtrapolateStickVlaues = true;
	//	//Serial.print(com.received.steer.LR);
	//	//Serial.print(" ");
	//	//Serial.println(extrapolatedLRstick);






	//	/* OVERRIDE THAT CODE WITH THE NEW FLIGHT MODES

	//	fModes::runVirtualPilot();


	//	// when pilot is disarmed motors will not spin
	//	// when disconnected form the pilot, motors will stop (not enabled)

	//	motors.setOnTL(fModes::vSticks.throttle + pidXval + pidYval - pidYawVal); // BR
	//	motors.setOnTR(fModes::vSticks.throttle + pidXval - pidYval + pidYawVal); // BL
	//	motors.setOnBR((int16_t)(fModes::vSticks.throttle * 1.5f) - pidXval - pidYval - pidYawVal); // TL (damaged)
	//	motors.setOnBL(fModes::vSticks.throttle - pidXval + pidYval + pidYawVal); // TR
	//	motors.forceMotorsExecution();

	//	*/
	//}







	/*

	!!!!!!!!!!!!!

	THIS HAVE TO BE IMPLEMENTED IN THE NEW FLIGHT MODE CLASSES


	void updatePressureAndAltHold()
	{
		using namespace config;

		// If altHold flight mode is enabled (this flag is set in the flight modes header file
		if (flags.needToUpdateAltHoldPID)
		{
			// calculate pressureToHold
			int16_t rawAltHoldThrottle = com.received.steer.throttle - altHoldBaseThrottle;
			// integrate the stick value only if 
			if (com.connectionStability() > 1)
			{
				// if raw throttle stick is out of the dead zone, integrate pressureToHold
				// !!  1/100Hz ~= 0.009   !!!  ONLY IF 110Hz  !!!
				if (rawAltHoldThrottle > flModeConfig.AltHoldStickDeadZone)
					pressureToHold -= ((float)(rawAltHoldThrottle - flModeConfig.AltHoldStickDeadZone) / 50.0f) * 0.009f;
				else if (rawAltHoldThrottle < -flModeConfig.AltHoldStickDeadZone)
					pressureToHold -= ((float)(rawAltHoldThrottle + flModeConfig.AltHoldStickDeadZone) / 50.0f) * 0.009f;
			}


			float altError = baro.getSmoothPressure() - pressureToHold;
			lastPID_AltHold_value = altHoldPID.updateController(altError);

			// keep pid value in a border
			lastPID_AltHold_value = constrain(lastPID_AltHold_value, -flModeConfig.AltHoldMaxAddedThrottle, flModeConfig.AltHoldMaxAddedThrottle);

			// apply pid results to the virtual throttle stick
			int16_t throttleStickSigned = altHoldBaseThrottle + lastPID_AltHold_value;
			fModes::vSticks.throttle = constrain(throttleStickSigned,
				flModeConfig.AltHoldMinTotalFinal,
				flModeConfig.AltHoldMaxTotalFinal);
		}
	}



	*/





	void UpdateSending::execute()
	{
		// Pack all data to the toSend variables
		com.toSend.tilt_TB = (int8_t)angle.x;
		com.toSend.tilt_LR = (int8_t)angle.y;
		com.toSend.altitude = (int16_t)(baro.getSmoothPressure() - 90000);


		// Send proper data packet: TYPE1-full, TYPE2-basic
		//com.packAndSendData(com.sendPacketTypes.TYPE2_ID, com.sendPacketTypes.TYPE2_SIZE); // basic
		com.packAndSendData(com.sendPacketTypes.TYPE1_ID, com.sendPacketTypes.TYPE1_SIZE); // full




		/* /////////////////////////
		Serial.print("H: ");
		Serial.print(heading);
		Serial.println();
		*/

		/*
		//Serial.println((uint16_t)com.received.steer.throttle);
		Serial.print(com.connectionStability());
		Serial.print("\t");
		Serial.print(angle.x);
		Serial.print("\t");
		Serial.print(angle.y);
		Serial.println();
		*/

		//Serial.println(com.toSend.tilt_TB);

		//Serial.println(MesasureTime::duration());
		//Serial.println(heading);
	}


	void UpdateReceiving::execute()
	{
		// update temporary previous stick values for extrapolation
		// those values will be stored if new one will come
		int16_t tempPreviousTBvalue = com.received.steer.TB;
		int16_t tempprevioudLRvalue = com.received.steer.LR;



		// If at least one data packet was received
		if (com.receiveAndUnpackData())
		{
			static bool lastArmingState = 0;

			// check if pilot changed armed state from 0 to 1
			if (lastArmingState == 0 && com.received.arming == 1)
			{
				lastArmingState = 1;
				levelXpid.resetController();
				levelYpid.resetController();
				yawPID.resetController();

				//    RESET ALL PID CONTROLLERS  !!!
				// and do all code when arming

				motors.setMotorState(true);
			}


			if (com.received.arming == 0)
			{
				lastArmingState = 0;
				motors.setMotorState(false);
			}


			// if any PID params was received
			if (com.wasReceived(com.receivedPacketTypes.TYPE3_ID))
			{
				switch (com.received.PIDcontrollerID)
				{
				case 0: // leveling
					// x
					levelXpid.set_kP(com.received.PIDvalues.P);
					levelXpid.set_kI(com.received.PIDvalues.I);
					levelXpid.set_Imax(com.received.PIDvalues.I_max);
					levelXpid.set_kD(com.received.PIDvalues.D);
					// y
					levelYpid.set_kP(com.received.PIDvalues.P);
					levelYpid.set_kI(com.received.PIDvalues.I);
					levelYpid.set_Imax(com.received.PIDvalues.I_max);
					levelYpid.set_kD(com.received.PIDvalues.D);
					break;


				case 1: // yaw
					yawPID.set_kP(com.received.PIDvalues.P);
					yawPID.set_kI(com.received.PIDvalues.I);
					yawPID.set_Imax(com.received.PIDvalues.I_max);
					yawPID.set_kD(com.received.PIDvalues.D);
					break;


				case 2: // altHold
					altHoldPID.set_kP(com.received.PIDvalues.P);
					altHoldPID.set_kI(com.received.PIDvalues.I);
					altHoldPID.set_Imax(com.received.PIDvalues.I_max);
					altHoldPID.set_kD(com.received.PIDvalues.D);



					// other cases.....
					// ...
					// ...
				}
			}
		}


		/*
		// WHEN LOST THE SIGNAL, then disable motors

		IMPLEMENT THIS INSIDE THE NEW FAILSAFE CLASS   !!!!!

		if (com.connectionStability() == 0)
		{
			motors.setMotorState(false);
		}*/


		// light up the red diode
		digitalWrite(config::pin.redDiode, (com.connectionStability() >= 1) ? HIGH : LOW);
	}



}

