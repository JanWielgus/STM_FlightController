/*
    Name:       FC_MPU6050Lib_example.ino
    Created:	04/06/2019 12:42:32
    Author:     Jan Wielgus
*/

#include "FC_MPU6050Lib.h"


FC_MPU6050Lib mpu;

FC_MPU6050Lib::vector3Int rawData;


void setup()
{
	Serial.begin(115200);
	Serial.println("Program has just started!");
	
	delay(3000);
	
	
	mpu.setFastClock();
	
	while (!mpu.initialize()) // While mpu is not initialized
	{
		// If program is there, some problems occured
		Serial.println("MPU6050 is cannot be initialized!");
		delay(500);
	}
	
	/* GYRO CALIBRATION !
	Serial.println("Started calibrating the gyro");
	mpu.calibrateGyro();
	Serial.println("Gyro calibration has finished");
	*/
}


void loop()
{
	mpu.read6AxisMotion();
	
	data = mpu.getRawAcceleration();
	Serial.print("X: ");
	Serial.print(data.x);
	Serial.println();
	
	
	delay(4); // 250Hz loop
}
