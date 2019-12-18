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
#include "MeasureTime.h"
#include "TaskerFunctions.h"
#include "DebugSystem.h"




void setup()
{
	// Communication serial
	Serial1.begin(BAUD_19200);
	
	// Debugging
	debug.begin(BAUD_115200);
	debug.enable();

	delay(1200); // Only for Atmel studio serial monitor
	debug.printHeader("Program has just started!");
	
	
	// on-board, blue and red diode
	pinMode(LED_BUILTIN, OUTPUT);
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


	// Add functions to the Tasker tasks
	addTaskerFunctionsToTasker();


	
	delay(300);
	debug.printHeader("Tasker done!");
	
	// MPU6050
	while (!mpu.initialize()) // While mpu is not initialized
	{
		// If gets stuck here, there is an error
		debug.printWarning("cannot initialize mpu");
		// DETECT MPU ERROR HERE
		delay(200);
	}
	
	mpu.setCalculationsFrequency(250);
	mpu.setGyroFusionMultiplier(0.999); // CHANGED TO TEST
	
	
	
	///////////////
	// TEMPORARY //
	///////////////
	debug.print("Started calibrations... ");
	
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
	
	debug.println(" PASSED");
	
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

