#include "TaskerFunctions.h"
#include "Storage.h"
#include "config.h"
#include <FC_Extrapolation.h>
#include <FC_LinearExtrapolation.h>
#include "Failsafe.h"
#include <LowPassFilter.h>

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


	tasker.addTask(new Update1Hz, 1000000L, 2);
	tasker.addTask(new Update75Hz, 13340L, 233);
	tasker.addTask(new UpdateMainInterval, config::MainInterval, 1000); // duration: less than 1000us in Stabilize
	tasker.addTask(new Failsafe, 50000L, 0); // 20Hz

	baro.registerNewBaroReadingFunction(newBaroReadingEvent);


	// Communication
	tasker.addTask(&comm, 4200L, 0); // (stale)220Hz ( !!! To solve. During communication some data is lost)
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
	// pressure filters
	LowPassFilter pressureLowPassFilter(1.3, 0.009329f); // average frequency: 107.2 Hz
	float pressureLPF_lowFreq = 0; // low-frequency (about 107Hz) pressure after low-pass filter
	FC_EVA_Filter pressureEVAFilter(0.3);

	// stick filters
	FC_EVA_Filter throttleFilter(0.5);
	FC_EVA_Filter rotateFilter(0.5);
	FC_EVA_Filter TB_fiter(0.58);
	FC_EVA_Filter LR_filter(0.58);



	void Update1Hz::execute()
	{
		updateControlDiode();
	}

	void Update75Hz::execute()
	{
		// Read raw data from compass
		Storage::rawHMC5883L.readRaw();
		
		//Serial.print(Storage::reading.pitch);
		//Serial.print('\t');
		//Serial.println(Storage::reading.roll);
		//Serial.print('\t');
		//Serial.println(Storage::reading.heading);
	}

	void UpdateMainInterval::execute()
	{
		// Read raw data from accelerometer and gyrscope
		Storage::rawMPU6050.readRawData();

		processSlowerReadings();

		ahrs.execute();

		Storage::virtualPilot.runVirtualPilot();
	}




	
	


	void updateControlDiode()
	{
		static bool ledState = LOW;

		// Blink built-in diode
		digitalWrite(config::pin.LedBuiltIn, ledState);
		ledState = !ledState;

		// Update communication indication diode
		digitalWrite(config::pin.redDiode, (comm.getConnectionStability() >= 75) ? HIGH : LOW);
	}


	void processSlowerReadings()
	{
		// Get current time from tasker (to speed up)
		uint32_t curTime = tasker.getCurrentTime();


		// update pressure
		reading.pressure = pressureEVAFilter.updateFilter(pressureLPF_lowFreq);

		// filter received sticks values
		Storage::sticksFiltered.throttle = throttleFilter.updateFilter(ReceiveData::throttle);
		Storage::sticksFiltered.rotate = rotateFilter.updateFilter((int16_t)ReceiveData::rot_stick);
		Storage::sticksFiltered.TB = TB_fiter.updateFilter((int16_t)ReceiveData::TB_stick);
		Storage::sticksFiltered.LR = LR_filter.updateFilter((int16_t)ReceiveData::LR_stick);
	}


	// average interval: 9329 us, average frequency: 107.2 Hz
	void newBaroReadingEvent()
	{
		// update filter on 107Hz pressure value (250Hz reading calculated in processSlowerReadings() method)
		pressureLPF_lowFreq = pressureLowPassFilter.update(baro.getPressure());
	}





	/* MAKE IT DIFFERENTLY
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





	void UpdateSending::execute()
	{
		// Pack all data to the toSend variables
		SendData::tilt_TB = (int8_t)reading.pitch;
		SendData::tilt_LR = (int8_t)reading.roll;
		SendData::heading = (int16_t)reading.heading;
		SendData::altitude = (int16_t)(reading.pressure - 90000 - 8530); // TEMP ! (change for altitude)
		SendData::receivingConnectionStability = comm.getConnectionStability();


		//comm.sendDataPacket(&SendData::DP_basic);
		comm.sendDataPacket(&SendData::DP_full);
	}







// Communication events


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

			case FlightModeType::ALT_HOLD:
				virtualPilot.setFlightMode(FlightModeType::ALT_HOLD);
				break;

			// TODO: other flight modes when will be implemented

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

		
		// make a beep
		digitalWrite(config::pin.buzzer, HIGH);
		delay(50);
		digitalWrite(config::pin.buzzer, LOW);
	}

}

