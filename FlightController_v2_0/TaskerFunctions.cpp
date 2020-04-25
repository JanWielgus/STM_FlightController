#include "TaskerFunctions.h"
#include "Storage.h"
#include "config.h"
#include <FC_Extrapolation.h>
#include <FC_LinearExtrapolation.h>

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
	tasker.addTask(new UpdateControlDiode, 1000000L, 2);							// 1Hz

	// Steering
	tasker.addTask(new ReadXY_angles, config::MainInterval, 639);				// 250Hz
	tasker.addTask(new ReadCompass, 13340L, 492);								// 75Hz
	baro.registerNewBaroReadingFunction(newBaroReadingEvent);
	tasker.addTask(new ProcessSlowerReadings, config::MainInterval, 0);			// 250Hz
	tasker.addTask(new RunVirtualPilot, config::MainInterval, 0);	// 250Hz


	// Communication
	tasker.addTask(&comm, 4545L, 0); // 220Hz ( !!! To solve. During communication some data is lost)
	tasker.addTask(new UpdateSending, 22000L, 0);								// ~45Hz

	// add receive data packets
	comm.addRaceiveDataPacketPointer(&ReceiveData::DP_steering, 35); // pointer, queue size
	comm.addRaceiveDataPacketPointer(&ReceiveData::DP_basicBackground, 8);
	comm.addRaceiveDataPacketPointer(&ReceiveData::DP_fullBackground, 8);
	comm.addRaceiveDataPacketPointer(&ReceiveData::DP_PID_params, 3);

	// received packet events
	ReceiveData::DP_steering.setPacketEvent(new SteeringReceivedUpdate);
	ReceiveData::DP_basicBackground.setPacketEvent(new BasicBackgroundReceivedUpdate);
	ReceiveData::DP_fullBackground.setPacketEvent(new FullBackgroundReceivedUpdate);
	ReceiveData::DP_PID_params.setPacketEvent(new PID_paramsReceivedUpdate);
}



namespace TaskerFunction
{
	FC_Extrapolation* compassExtrapolator = new FC_LinearExtrapolation();
	FC_Extrapolation* baroExtrapolator = new FC_LinearExtrapolation();

	FC_EVA_Filter throttleFilter(0.5);
	FC_EVA_Filter rotateFilter(0.5);
	FC_EVA_Filter TB_fiter(0.58);
	FC_EVA_Filter LR_filter(0.58);


	void UpdateControlDiode::execute()
	{
		// Blink built-in diode
		digitalWrite(config::pin.LedBuiltIn, ledState);
		ledState = !ledState;

		// Update communication indication diode
		digitalWrite(config::pin.redDiode, (comm.getConnectionStability() >= 75) ? HIGH : LOW);
	}

	/* DO IT DIFFERENTLY
		Make separate procedure where calibraiton will be performed
		Calibration will be triggered in receive update task which is called after receiving specific packet type
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
	}*/


	void ReadXY_angles::execute()
	{
		mpu.read6AxisMotion();
		reading.angle = mpu.getFusedXYAngles();
		
		if (config::booleans.UseCompassInZAxisAngleCalculation)
			// Return compass heading extrapolation for the current time
			reading.heading = mpu.getZAngle(reading.compassHeading);
		else
			reading.heading = mpu.getZAngle();
	}


	void ReadCompass::execute()
	{
		compass.readCompassData(reading.angle.x, reading.angle.y);

		// Add new compass heading measurement for the current time
		compassExtrapolator->addNewMeasuredValue(compass.getHeading(), tasker.getCurrentTime());
	}


	void newBaroReadingEvent()
	{
		// normal pressure is just assigned to the globar reading variable
		reading.pressure = baro.getPressure();

		// smooth pressure is extrapolated
		baroExtrapolator->addNewMeasuredValue(baro.getSmoothPressure(), tasker.getCurrentTime());
	}


	void ProcessSlowerReadings::execute()
	{
		// Get current time from tasker (to speed up)
		uint32_t curTime = tasker.getCurrentTime();


		// extrapolate compass reading
		reading.compassHeading = compassExtrapolator->getEstimation(curTime);


		// extrapolate baro reading to meet the program main frequency (250Hz)
		reading.smoothPressure = baroExtrapolator->getEstimation(curTime);


		// filter received sticks values
		Storage::sticksFiltered.throttle = throttleFilter.updateFilter(ReceiveData::throttle);
		Storage::sticksFiltered.rotate = rotateFilter.updateFilter(ReceiveData::rot_stick);
		Storage::sticksFiltered.TB = TB_fiter.updateFilter(ReceiveData::TB_stick);
		Storage::sticksFiltered.LR = LR_filter.updateFilter(ReceiveData::LR_stick);

		//Serial.println(Storage::sticksFiltered.TB);
	}


	void RunVirtualPilot::execute()
	{
		Storage::virtualPilot.runVirtualPilot();
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
		SendData::tilt_TB = (int8_t)reading.angle.x;
		SendData::tilt_LR = (int8_t)reading.angle.y;
		SendData::heading = (int16_t)reading.heading;
		SendData::altitude = (int16_t)(baro.getSmoothPressure() - 90000); // TEMP ! (change for altitude)
		SendData::receivingConnectionStability = comm.getConnectionStability();


		//comm.sendDataPacket(&SendData::DP_basic);
		comm.sendDataPacket(&SendData::DP_full);
	}


	void SteeringReceivedUpdate::execute()
	{
		// stuff after receiving steering values
		// ...
	}


	void BasicBackgroundReceivedUpdate::execute()
	{
		// Update flight mode
		// If flight mode has changed
		if (virtualPilot.getCurrentFlightModeType() != ReceiveData::flightMode)
		{
			switch (ReceiveData::flightMode)
			{
			case FlightModeType::UNARMED:
				virtualPilot.setFlightMode(FlightModeType::UNARMED);
				break;

			case FlightModeType::STABILIZE:
				virtualPilot.setFlightMode(FlightModeType::STABILIZE);
				break;

			default:
				virtualPilot.setFlightMode(FlightModeType::UNARMED);
			}
		}


		// other stuff
		// ...
	}


	void FullBackgroundReceivedUpdate::execute()
	{
		// call basic update
		BasicBackgroundReceivedUpdate::execute();

		// and addidional
		// ...

	}


	void PID_paramsReceivedUpdate::execute()
	{
		// !!!
		// 
		// I think, that should be made in a different way
		// For example there only calibration values are updated inside config
		// They are stored in EEPROM and other funciton sets proper values


		switch (ReceiveData::tunedControllerID)
		{
		case 0: // leveling
			// x
			levelXpid.set_kP(ReceiveData::tunedPID_values.P);
			levelXpid.set_kI(ReceiveData::tunedPID_values.I);
			levelXpid.set_Imax(ReceiveData::tunedPID_values.I_max);
			levelXpid.set_kD(ReceiveData::tunedPID_values.D);
			// y
			levelYpid.set_kP(ReceiveData::tunedPID_values.P);
			levelYpid.set_kI(ReceiveData::tunedPID_values.I);
			levelYpid.set_Imax(ReceiveData::tunedPID_values.I_max);
			levelYpid.set_kD(ReceiveData::tunedPID_values.D);
			break;


		case 1: // yaw
			yawPID.set_kP(ReceiveData::tunedPID_values.P);
			yawPID.set_kI(ReceiveData::tunedPID_values.I);
			yawPID.set_Imax(ReceiveData::tunedPID_values.I_max);
			yawPID.set_kD(ReceiveData::tunedPID_values.D);
			break;


		case 2: // altHold
			altHoldPID.set_kP(ReceiveData::tunedPID_values.P);
			altHoldPID.set_kI(ReceiveData::tunedPID_values.I);
			altHoldPID.set_Imax(ReceiveData::tunedPID_values.I_max);
			altHoldPID.set_kD(ReceiveData::tunedPID_values.D);
		}
	}

}

