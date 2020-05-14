/*
    Name:       FlightController_v2_0.ino
    Created:	30/05/2019 22:12:18
    Author:     Jan Wielgus
	
	Source code of second version of the FlightController on the STM BluePill micro-controller.
*/


#include <FC_Communication_Base.h>
#include <FC_CommunicationHandler.h>
#include "CommSendDataPackets.h"
#include "CommRecDataPackets.h"
#include "Interfaces.h"
#include <FC_Task.h>
#include <FC_ObjectTasker.h>
#include <FC_Extrapolation.h>
#include <FC_LinearExtrapolation.h>
#include <MyPID.h>
#include "MPU6050_Raw.h"
#include "HMC5883L_Raw.h"
#include "Norm3AxisDataAdapters.h"
#include "AHRS_Method.h"
#include "AHRS.h"
#include <FC_MS5611_Lib.h>
#include <FC_EVA_Filter.h>
#include <Wire.h>
#include <FC_CustomDataTypes.h>
#include <FC_TaskPlanner.h>
#include <FC_AverageFilter.h>
#include <FC_SinkingQueue.h>
#include <FC_GrowingArray.h>
#include "Storage.h"
#include "FC_Motors.h"
#include "TaskerFunctions.h"
#include "DebugSystem.h"
#include "config.h"
#include "VirtualPilot.h"
#include "FlightMode.h"
#include "StabilizeFlightMode.h"
#include "AltHoldFlightMode.h"
#include "PosHoldFlightMode.h"
#include "UnarmedFlightMode.h"
#include "SharedDataTypes.h"
#include "Failsafe.h"


using namespace Storage;




void setup()
{
	// Communication serial
	Serial2.begin(BAUD_57600);
	
	// Debugging
	debug.begin(BAUD_115200);
	debug.enable();

	delay(1200); // Only for Atmel studio serial monitor
	debug.printHeader("Program has just started!");
	
	

	// on-board, blue and red diode
	pinMode(config::pin.LedBuiltIn, OUTPUT);
	pinMode(config::pin.redDiode, OUTPUT);
	pinMode(config::pin.blueDiode, OUTPUT);
	

	
	// TEMPORARY !!!  set 
	pinMode(config::pin.m0pin, OUTPUT);
	pinMode(config::pin.m1pin, OUTPUT);
	pinMode(config::pin.aux, INPUT);
	digitalWrite(config::pin.m0pin, HIGH);
	digitalWrite(config::pin.m1pin, LOW);



	// Set the startup flight mode
	while (!virtualPilot.setFlightMode(FlightModeType::UNARMED))
	{
		debug.printHeader("Base flight mode NOT SET");
		delay(500);
	}

	
	
	// set motors to zero power
	motors.setOnTL(0);
	motors.setOnTR(0);
	motors.setOnBR(0);
	motors.setOnBL(0);
	motors.setMotorState(false); // disable motors



	// Add functions to the Tasker tasks
	addTaskerFunctionsToTasker();

	// Makes that time has not influence connection stability value
	// Use after adding tasks to tasker
	//comm.adaptConStabFilterToInterval();
	comm.setConStabFilterIntensity(0.95f); // set filter intensity manually
	
	delay(300);
	debug.printHeader("Tasker done!");




	Wire.begin();
	

	// MPU6050
	while (!rawMPU6050.initialize())
	{
		// If gets stuck here, there is an error
		debug.printWarning("cannot initialize mpu");
		// DETECT MPU ERROR HERE
		delay(300);
	}
	rawMPU6050.enableCompassBypass(); // enable compass on GY-86

	Serial.println("mpu initialized");


	// HMC5003L
	while (!rawHMC5883L.initialize())
	{
		// If gets stuck here, there is an error
		Serial.println("cannot initialize compass");
		// DETECT COMPASS ERROR HERE
		delay(200);
	}

	Serial.println("compass initialized");


	// MS5611
	while (!baro.initialize(false)) // initialize baro without Wire.begin()
	{
		// If gets stuck here, there is an error
		Serial.println("cannot initialize baro");
		delay(200);
	}

	Serial.println("baro initialized");



	// Set I2C clock to 400kHz
	Wire.setClock(400000L);

	

	
	///////////////
	// TEMPORARY //
	///////////////
	// Uncomment one, to perform calibration
	debug.print("Started calibrations... ");
	
	/*
	//delay(1500);
	digitalWrite(config::pin.blueDiode, HIGH);
	rawMPU6050.calibrateAccelerometer(2000);
	digitalWrite(config::pin.blueDiode, LOW);
	while (true)
	{
		Serial.print("Acc done. X: ");
		Serial.print(rawMPU6050.getAccOffset().x);
		Serial.print(" Y: ");
		Serial.print(rawMPU6050.getAccOffset().y);
		Serial.print(" Z: ");
		Serial.print(rawMPU6050.getAccOffset().z);
		Serial.println();
		delay(2000);
	}/*/
	rawMPU6050.setAccOffset(142, 82, -227); // <<<<<<<<<<<<< TO SET
	
	
	/*
	//delay(1500);
	digitalWrite(config::pin.blueDiode, HIGH);
	rawMPU6050.calibrateGyroscope(6000);
	digitalWrite(config::pin.blueDiode, LOW);
	while (true)
	{
		Serial.print("Gyro done. X: ");
		Serial.print(rawMPU6050.getGyroOffset().x);
		Serial.print(" Y: ");
		Serial.print(rawMPU6050.getGyroOffset().y);
		Serial.print(" Z: ");
		Serial.print(rawMPU6050.getGyroOffset().z);
		Serial.println();
		delay(2000);
	}/*/
	rawMPU6050.setGyroOffset(-145, 110, -1); // <<<<<<<<<<<<< TO SET



	/*
	//delay(1500);
	digitalWrite(config::pin.blueDiode, HIGH);
	rawHMC5883L.calibrate(60);
	digitalWrite(config::pin.blueDiode, LOW);
	while (true)
	{
		Serial.print("Compass done. X: ");
		Serial.print(rawHMC5883L.getCompassOffset().x);
		Serial.print(" Y: ");
		Serial.print(rawHMC5883L.getCompassOffset().y);
		Serial.print(" Z: ");
		Serial.print(rawHMC5883L.getCompassOffset().z);
		Serial.println();
		delay(2000);
	}/*/
	rawHMC5883L.setCompassOffset(-192, -802, 1425); // <<<<<<<<<<<<< TO SET


	debug.println(" PASSED");



	// set up PID derivative low-pass filters
	Storage::levelXpid.setDerivativeLowPassFilterParams(6);
	Storage::levelYpid.setDerivativeLowPassFilterParams(6);
	Storage::yawPID.setDerivativeLowPassFilterParams(6);
	Storage::altHoldPID.setDerivativeLowPassFilterParams(6);

	
	Serial.println("setup done");
}




void loop()
{
	// This is the only two things inside the loop()
	// Every other function/method/task have to use one of this objects
	tasker.run();
	taskPlanner.runPlanner();
}

