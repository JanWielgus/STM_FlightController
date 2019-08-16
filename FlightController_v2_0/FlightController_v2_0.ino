#include "FC_MainCommunication.h"
/*
    Name:       FlightController_v2_0.ino
    Created:	30/05/2019 22:12:18
    Author:     Jan Wielgus
	
	Source code of second version of the FlightController on the STM BluePill microcontroller.
*/


#include "Storage.h"


// Functions run by Tasker
void readXY_angles();
void readCompass();
void stabilize();
void updateSending();
void updateReceiving();
void checkCalibrations(); // Check if there is a need to calibrate one of the module and perform it if needed
void updateControlDiode(); // built in diode is blinked once per second

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
	tasker.addFunction(updateReceiving, 12500L, 1);            // 80Hz
	tasker.addFunction(checkCalibrations, 700000L, 7);         // 1.4Hz
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
	mpu.setGyroFusionMultiplier(0.996); // CHANGED TO TEST
	
	
	
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
	mpu.setAccelerometerCalibrationValues(397, 99, -227);
	
	
	/*
	mpu.calibrateGyro(4000);
	Serial.print("Gyro done. X: ");
	Serial.print(mpu.getGyroCalibrationValues().x);
	Serial.print(" Y: ");
	Serial.print(mpu.getGyroCalibrationValues().y);
	Serial.print(" Z: ");
	Serial.print(mpu.getGyroCalibrationValues().z);
	Serial.println();*/
	mpu.setGyroCalibrationValues(-104, -159, -9);
	
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
	//...
	// use angle and heading variables
	// use PID class
	
	
	extrapolateSticks();
	
	
	int16_t pidXval, pidYval, pidYawVal;
	
	// leveling PID
	pidXval = levelXpid.updateController(angle.x + (extrapolatedTBstick/10)) + 0.5;
	pidYval = levelYpid.updateController(angle.y - (extrapolatedLRstick/10)) + 0.5;
		
		
	// yaw PID
	float headingError = headingToHold - heading;
	
	if (headingError >= 180)
		headingError -= 360;
	else if (headingError <= -180)
		headingError += 360;
		
	pidYawVal = yawPID.updateController(headingError);
	
	//Serial.println(headingError);
	
	
	
	if (com.received.steer.throttle < 20)
	{
		pidXval = 0;
		pidYval = 0;
		pidYawVal = 0;
		
		headingToHold = heading;
	}
	
	
	// when pilot is disarmed motors will not spin
	// when disconnected form the pilot, motors will stop (not enabled)
	
 	motors.setOnTL(com.received.steer.throttle + pidXval + pidYval - pidYawVal); // BR (damaged)
 	motors.setOnTR(com.received.steer.throttle + pidXval - pidYval + pidYawVal); // BL
 	motors.setOnBR(com.received.steer.throttle - pidXval - pidYval - pidYawVal); // TL
 	motors.setOnBL(com.received.steer.throttle - pidXval + pidYval + pidYawVal); // TR
	motors.forceMotorsExecution();
}


void updateSending()
{
	// send proper data packet: TYPE1-full, TYPE2-basic
	com.toSend.tilt_TB = (int8_t)angle.x;
	com.toSend.tilt_LR = (int8_t)angle.y;
	//com.packAndSendData(com.sendPacketTypes.TYPE2_ID, com.sendPacketTypes.TYPE2_SIZE);
	com.packAndSendData(com.sendPacketTypes.TYPE1_ID, com.sendPacketTypes.TYPE1_SIZE);
	
	/*
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
				
				
				
				// other cases.....
				// ...
				// ...
			}
		}
	}
	
	
	// Integrate yaw stick value only if connection is stable
	if (com.connectionStability() > 1)
	{
		//headingToHold += ((float)com.received.steer.rotate * 0.04); // if 25Hz  !!!!!!!!!!!!!!!!!!!!!!!!!  ONLY
		headingToHold += ((float)(com.received.steer.rotate/2) * 0.0125); // if 80Hz  !!!!!!!!!!!!!!!!!!!!!!!!!  ONLY
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
