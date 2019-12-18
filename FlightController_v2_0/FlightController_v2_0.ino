/*
    Name:       FlightController_v2_0.ino
    Created:	30/05/2019 22:12:18
    Author:     Jan Wielgus
	
	Source code of second version of the FlightController on the STM BluePill micro-controller.
*/


#include <FC_Tasker.h>
#include <MyPID.h>
#include <FC_Communication_Base.h>
#include <FC_MPU6050Lib.h>
#include <FC_HMC5883L_Lib.h>
#include <FC_MS5611_Lib.h>
#include <FC_EVA_Filter.h>
#include <FC_Motors.h>
#include <Wire.h>
#include <FC_CustomDataTypes.h>
#include <FC_TaskPlanner.h>
#include <FC_AverageFilter.h>
#include "Storage.h"
#include "FlightModes.h"


// Functions run by Tasker
void readXY_angles();
void readCompass();
void stabilize();
void updateSending();
void updateReceiving();
void checkCalibrations(); // Check if there is a need to calibrate one of the module and perform it if needed
void updateControlDiode(); // built in diode is blinked once per second
void updatePressureAndAltHold(); // update altHold PID if needed and do other baro stuff

// Other functions
void extrapolateSticks();




class MesasureTime
{
	public:
	
	static uint32_t startTime;
	static uint32_t dur;
	
 public:
	static void start()
	{
		startTime = micros();
	}
	
	static void end()
	{
		dur = micros() - startTime;
	}
	
	static uint16_t duration()
	{
		return dur;
	}
};
uint32_t MesasureTime::startTime = 0;
uint32_t MesasureTime::dur = 0;

int16_t counter = 0; // Used to measure how many times function does in a second (checked in the updateControlDiode() )



void setup()
{
	// Communication serial
	Serial1.begin(19200);
	
	// Debugging
	Serial.begin(115200);
	delay(1200); // Only for Atmel studio serial monitor
	Serial.println("Program has just started!");
	
	pinMode(LED_BUILTIN, OUTPUT);
	
	// blue and red diode
	pinMode(config::pin.redDiode, OUTPUT);
	pinMode(config::pin.blueDiode, OUTPUT);
	
	
	// TEMPORARY !!!  set 
	pinMode(config::pin.m0pin, OUTPUT);
	pinMode(config::pin.m1pin, OUTPUT);
	pinMode(config::pin.aux, INPUT);
	digitalWrite(config::pin.m0pin, HIGH);
	digitalWrite(config::pin.m1pin, LOW);
	
	
	// set motors to zero power
	motors.setOnTL(0);
	motors.setOnTR(0);
	motors.setOnBR(0);
	motors.setOnBL(0);
	motors.setMotorState(false); // disable motors
	
	
	// default values
	angle.x = 0;
	angle.y = 0;
	angle.z = 0; // not used in this variable
	heading = 0;
	
	// Add functions to the Tasker tasks
	tasker.addFunction(stabilize, 4000L, 31);                  // 250Hz (duration tested only with leveling)
	tasker.addFunction(updateControlDiode, 1000000L, 2);       // 1Hz (tested duration)
	tasker.addFunction(readXY_angles, 4000L, 639);             // 250Hz (tested duration)
	tasker.addFunction(readCompass, 13340L, 492);              // 75Hz  (tested duration)
	//tasker.addFunction(updateMainCommunication, 20000L, 229);  // 50Hz (tested duration)
	tasker.addFunction(updateSending, 22000L, 1);              // ~45Hz
	tasker.addFunction(updateReceiving, 7142L, 1);             // 140Hz      (! UPDATE the body if frequency chanded !!!)
	tasker.addFunction(checkCalibrations, 700000L, 7);         // 1.4Hz
	tasker.addFunction(updatePressureAndAltHold, 9090, 1);     // 110Hz
	//tasker.scheduleTasks();
	
	delay(300);
	Serial.println("tasker done");
	
	// MPU6050
	while (!mpu.initialize()) // While mpu is not initialized
	{
		// If gets stuck here, there is an error
		Serial.println("cannot initialize mpu");
		// DETECT MPU ERROR HERE
		delay(200);
	}
	
	mpu.setCalculationsFrequency(250);
	mpu.setGyroFusionMultiplier(0.999); // CHANGED TO TEST
	
	
	
	///////////////
	// TEMPORARY //
	///////////////
	Serial.print("Started calibrations... ");
	
	/*
	mpu.calibrateAccelerometer(1000); // 
	Serial.print("Acc done. X: ");
	Serial.print(mpu.getAccelerometerCalibrationValues().x);
	Serial.print(" Y: ");
	Serial.print(mpu.getAccelerometerCalibrationValues().y);
	Serial.print(" Z: ");
	Serial.print(mpu.getAccelerometerCalibrationValues().z);
	Serial.println();*/
	mpu.setAccelerometerCalibrationValues(78, -3, -243);
	
	
	/*
	mpu.calibrateGyro(4000);
	Serial.print("Gyro done. X: ");
	Serial.print(mpu.getGyroCalibrationValues().x);
	Serial.print(" Y: ");
	Serial.print(mpu.getGyroCalibrationValues().y);
	Serial.print(" Z: ");
	Serial.print(mpu.getGyroCalibrationValues().z);
	Serial.println();*/
	mpu.setGyroCalibrationValues(-108, -156, 0);
	
	Serial.println(" OK");
	
	///////////////
	
	
	
	Serial.println("mpu initialized");
	
	// HMC5003L
	compass.enableHMC_on_MPU(false); // delete this parameter. There is no need because wire have to be initialized before mpu starts
	while (!compass.initialize(false))
	{
		// If gets stuck here, there is an error
		Serial.println("cannot initialize compass");
		// DETECT COMPASS ERROR HERE
		delay(200);
	}
	
	compass.setCompassDeclination(5.0);
	Serial.println("compass initialized");


	// MS5611
	while (!baro.initialize())
	{
		// If gets stuck here, there is an error
		Serial.println("cannot initialize baro");
		delay(200);
	}

	Serial.println("baro initialized");
	
	
	// Default calibration values
	// When pilot needs other values at the beginning, it just send them (or request calibration)
	// and the whole process is repeated
	//mpu.setAccelerometerCalibrationValues(....);
	//setGyroCalibrationMethod here <----
	compass.setCalibrationValues(config::calibVal.compassMin, config::calibVal.compassMax);
	//compass.calibrateCompass(15);
	
	
	mpu.setFastClock(); // 400 kHz
	
	// set initial Z axis value
	mpu.read6AxisMotion();
	angle = mpu.getFusedXYAngles();
	compass.readCompassData(angle.x, angle.y);
	mpu.setInitialZAxisValue(compass.getHeading());
	
	Serial.println("setup done");
}




void loop()
{
	tasker.runTasker();
	baro.runBarometer(); // this method uses planned tasks which need to be checked as fast as possible
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void updateControlDiode()
{
	static bool ledState = LOW;
	digitalWrite(LED_BUILTIN, ledState);
	ledState = !ledState;
	
	//Serial.println(counter);
	counter = 0;
}



void readXY_angles()
{
	
	mpu.read6AxisMotion();
	angle = mpu.getFusedXYAngles();
	//heading = mpu.getZAngle(compass.getHeading()); // Temporary (something bad is happening with the compass)
	heading = mpu.getZAngle();
}


void readCompass()
{
	compass.readCompassData(angle.x, angle.y);
}


void stabilize()
{
	counter++;
	

	// Cut-off all motors if the angle is too high
	using namespace config;
	if (angle.x > CutOffAngle || angle.x < -CutOffAngle ||
		angle.y > CutOffAngle || angle.y < -CutOffAngle)
		motors.setMotorState(false);

	
	extrapolateSticks();
	fModes::runVirtualPilot();
	
	
	// when pilot is disarmed motors will not spin
	// when disconnected form the pilot, motors will stop (not enabled)

	motors.setOnTL(fModes::vSticks.throttle + pidXval + pidYval - pidYawVal); // BR
 	motors.setOnTR(fModes::vSticks.throttle + pidXval - pidYval + pidYawVal); // BL
 	motors.setOnBR((int16_t)(fModes::vSticks.throttle*1.5f) - pidXval - pidYval - pidYawVal); // TL (damaged)
 	motors.setOnBL(fModes::vSticks.throttle - pidXval + pidYval + pidYawVal); // TR
	motors.forceMotorsExecution();
}


void updatePressureAndAltHold()
{
	// If altHold flight mode is enabled (this flag is set in the flight modes header file
	if (needToUpdateAltHoldPID_flag)
	{
		// calculate pressureToHold
		int16_t rawAltHoldThrottle = com.received.steer.throttle - altHoldBaseThrottle;
		// integrate the stick value only if 
		if (com.connectionStability() > 1)
		{
			// if raw throttle stick is out of the dead zone, integrate pressureToHold
			// !!  1/100Hz ~= 0.009   !!!  ONLY IF 110Hz  !!!
			if (rawAltHoldThrottle > config::AltHoldStickDeadZone)
				pressureToHold -= ((float)(rawAltHoldThrottle - config::AltHoldStickDeadZone) / 50.0f) * 0.009f;
			else if (rawAltHoldThrottle < -config::AltHoldStickDeadZone)
				pressureToHold -= ((float)(rawAltHoldThrottle + config::AltHoldStickDeadZone) / 50.0f) * 0.009f;
		}


		float altError = baro.getSmoothPressure() - pressureToHold;
		pidAltHoldVal = altHoldPID.updateController(altError);

		// keep pid value in a border
		pidAltHoldVal = constrain(pidAltHoldVal, -config::AltHoldMaxAddedThrottle, config::AltHoldMaxAddedThrottle);

		// apply pid results to the virtual throttle stick
		int16_t throttleStickSigned = altHoldBaseThrottle + pidAltHoldVal;
		fModes::vSticks.throttle = constrain(throttleStickSigned,
											config::AltHoldMinTotalFinal,
											config::AltHoldMaxTotalFinal);
	}
}


void updateSending()
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


void updateReceiving()
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
		
		
		// set the extrapolation flag if new stick values was received
		if (com.wasReceived(com.receivedPacketTypes.TYPE4_ID))
		{
			// store previous stick values because new ones was received
			previousTBvalue = tempPreviousTBvalue;
			previousLRvalue = tempprevioudLRvalue;
			needToExtrapolateStickVlauesFlag = false;
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
	
	
	// Integrate yaw stick value only if connection is stable
	if (com.connectionStability() > 1)
	{
		//headingToHold += ((float)com.received.steer.rotate * 0.04f); // if 25Hz  !!!!!!!!!!!!!!!!!!!!!!!!!  ONLY
		//headingToHold += ((float)(com.received.steer.rotate/2) * 0.0125f); // if 80Hz  !!!!!!!!!!!!!!!!!!!!!!!!!  ONLY
		headingToHold += ((float)(com.received.steer.rotate / 2) * 0.0071f); // if 140Hz  !!!!!!!!!!!!!!!!!!!!!!!!!  ONLY
	}
	
	
	/*
	// WHEN LOST THE SIGNAL, then disable motors
	if (com.connectionStability() == 0)
	{
		motors.setMotorState(false);
	}*/
	
	
	// light up the red diode
	digitalWrite(config::pin.redDiode, (com.connectionStability() >= 1) ? HIGH : LOW );
}


void checkCalibrations()
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


void extrapolateSticks()
{
	// Extrapolate TB and LR stick values
	if (needToExtrapolateStickVlauesFlag)
	{
		extrapolatedTBstick += (float(com.received.steer.TB - previousTBvalue) * 0.2);
		extrapolatedLRstick += (float(com.received.steer.LR - previousLRvalue) * 0.2);
	}
	else
	{
		extrapolatedTBstick = (float)com.received.steer.TB;
		extrapolatedLRstick = (float)com.received.steer.LR;
	}
	// next run should extrapolate sticks values unless communication will reset this flag
	extrapolatedTBstick = tbFilter.updateFilter(extrapolatedTBstick);
	extrapolatedLRstick = lrFilter.updateFilter(extrapolatedLRstick);
	needToExtrapolateStickVlauesFlag = true;
	//Serial.print(com.received.steer.LR);
	//Serial.print(" ");
	//Serial.println(extrapolatedLRstick);
}
