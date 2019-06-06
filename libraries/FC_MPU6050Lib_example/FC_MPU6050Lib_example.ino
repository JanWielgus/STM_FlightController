/*
    Name:       FC_MPU6050Lib_example.ino
    Created:	04/06/2019 12:42:32
    Author:     Jan Wielgus
*/

#include "FC_MPU6050Lib.h"


FC_MPU6050Lib mpu;

FC_MPU6050Lib::vector3Float data;

uint32_t loopStartTime;


void setup()
{
	Serial.begin(115200);
	Serial.println("Program has just started!");
	
	delay(3000);
	
	
	mpu.setFastClock(); // 400 kHz clock
	
	while (!mpu.initialize()) // While mpu is not initialized
	{
		// If program is there, some problems occured
		Serial.println("MPU6050 is cannot be initialized!");
		delay(500);
	}
	
	
	// !!!
	// If program is working in a frequency different than 250Hz
	// This method HAVE TO BE CALLED !!!!!
	// !!!
	mpu.setCalculationsFrequency(250);
	
	
	// You can also change gyro influence in the fusion angle
	// by calling a method
	//mpu.setGyroFusionMultiplier(0.996); // values between 0 and 1 (but not equal)
	
	
	// GYRO CALIBRATION !
	Serial.println("Started calibrating the gyro");
	mpu.calibrateGyro();
	Serial.println("Gyro calibration has finished");
	
	loopStartTime = micros();
}


void loop()
{
	mpu.read6AxisMotion();
	data = mpu.getFusedAngles(); // !!! This method have to be called in a certain frequency (default 250Hz) !!!
	//data = mpu.getAccAngles(); // alternative (very noisy measurement)
	Serial.print("X: ");
	Serial.print(data.x);
	Serial.print("\tY: ");
	Serial.print(data.y);
	Serial.println();
	
	
	//delay(4); // 250Hz loop
	while (micros()-loopStartTime <= 4000)
	{
		// wait
	}
	loopStartTime = micros();
}


/* How to get raw data
FC_MPU6050Lib::vector3Int rawData = mpu.getRawRotation();
FC_MPU6050Lib::vector3Int rawData = mpu.getRawAcceleration();
*/
