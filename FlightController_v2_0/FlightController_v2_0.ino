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
	mpu.calibrateAccelerometer(30); // 
	mpu.calibrateGyro(110);
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
	heading = mpu.getZAngle(compass.getHeading());
	
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
	
	int16_t pidXval, pidYval;
	
	// leveling PID
	pidXval = levelXpid.updateController(angle.x + (com.received.steer.TB/10)) + 0.5;
	//pidYval = levelYpid.updateController(angle.y - (com.received.steer.LR/10)) + 0.5;
		
		
	// yaw PID
	// ...
	
	
	//Serial.println(pidXval);
	
	
	if (com.received.steer.throttle < 20)
	{
		pidXval = 0;
		pidYval = 0;
	}
	
	
	// when pilot is disarmed motors will not spin
	// when disconnected form the pilot, motors will stop (not enabled)
	
 	motors.setOnTL(com.received.steer.throttle + pidXval + pidYval); // BR (damaged)
 	motors.setOnTR(com.received.steer.throttle + pidXval - pidYval); // BL
 	motors.setOnBR(com.received.steer.throttle - pidXval - pidYval); // TL
 	motors.setOnBL(com.received.steer.throttle - pidXval + pidYval); // TR
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
}


void updateReceiving()
{
	if (com.receiveAndUnpackData())
	{
		// check if pilot set armed state
		if (com.received.arming == 1)
		{
			levelXpid.resetController();
			levelYpid.resetController();
			//    RESET ALL PID CONTROLLERS  !!!
			// and do all code when arming
			
			motors.setMotorState(true);
		}
		else
		motors.setMotorState(false);
		
		headingToHold += ((float)com.received.steer.rotate * 0.04); // if 25Hz  !!!!!!!!!!!!!!!!!!!!!!!!!  ONLY
	}
	
	
	
	// WHEN LOST THE SIGNAL, then disable motors
	if (com.connectionStability() == 0)
	{
		motors.setMotorState(false);
	}
	
	
	// light up the red diode if connection stability is greater than 0
	digitalWrite(config::pin.redDiode, (com.connectionStability() >= 3) ? HIGH : LOW );
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
