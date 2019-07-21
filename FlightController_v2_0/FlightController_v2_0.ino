#include "FC_MainCommunication.h"
/*
    Name:       FlightController_v2_0.ino
    Created:	30/05/2019 22:12:18
    Author:     Jan Wielgus
	
	Source code of second version of the FlightController on the STM BluePill microcontroller.
*/


#include <FC_Tasker.h>
#include <MyPID.h>
#include <FC_Communication_Base.h>
#include <FC_MPU6050Lib.h>
#include <FC_HMC5883L_Lib.h>
#include <FC_EVA_Filter.h>
#include <FC_Motors.h>
#include "config.h"



// Functions run by Tasker
void readXY_angles();
void readCompass();
void stabilize();
void updateMainCommunication();
// Check if there is a need to calibrate one of the module and perform it if needed
void checkCalibrations();
void updateControlDiode(); // built in diode is blinked once per second


// create the tasker object
FC_SimpleTasker tasker;

// create the communication object
FC_MainCommunication com(&Serial1, 45);

// create sensors objects
FC_MPU6050Lib mpu;
FC_HMC5883L_Lib compass;
FC_MPU6050Lib::vector3Float angle; // X and Y angles
float heading;



void setup()
{
	// Cummunication serial
	Serial1.begin(9600);
	
	// Debugging
	Serial.begin(115200);
	delay(1200); // Only for Atmel studio serial monitor
	Serial.println("Program has just started!");
	
	pinMode(LED_BUILTIN, OUTPUT);
	
	
	// default values
	angle.x = 0;
	angle.y = 0;
	angle.z = 0; // not used in this variable
	heading = 0;
	
	// Add functions to the Tasker tasks
	tasker.addFunction(readXY_angles, 4000L, 639);             // 250Hz (tested duration)
	tasker.addFunction(readCompass, 13340L, 492);              // 75Hz  (tested duration)
	tasker.addFunction(stabilize, 4000L, 17);                  // 250Hz
	tasker.addFunction(updateMainCommunication, 40000L, 11);   // 25Hz
	tasker.addFunction(checkCalibrations, 700000L, 7);         // 1.4Hz
	tasker.addFunction(updateControlDiode, 1000000L, 5);       // 1Hz
	tasker.scheduleTasks();
	
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
	//...
	// use angle and heading variables
	// use PID class
}


void updateMainCommunication()
{
	com.receiveAndUnpackData();
	
	// send proper data packet: TYPE1-full, TYPE2-basic
	com.toSend.tilt_TB = (int8_t)angle.x;
	com.toSend.tilt_LR = (int8_t)angle.y;
	com.packAndSendData(com.sendPacketTypes.TYPE2_ID, com.sendPacketTypes.TYPE2_SIZE);
	
	/*
	Serial.print("H: ");
	Serial.print(heading);
	Serial.println();
	*/
	Serial.println((uint16_t)com.received.steer.throttle);
	//Serial.println(com.connectionStability());
	//Serial.println(com.toSend.tilt_TB);
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
