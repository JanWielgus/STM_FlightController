// Sensors.h
// Author: Jan Wielgus
// Date: 25.10.2017

#ifndef _SENSORS_h
#define _SENSORS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <I2Cdev.h>
#include <Wire.h>
#include "myMPU6050.h"
#include "myHMC5883L.h"

//#include <MS5611.h>
#include "config.h"


class SensorsClass
{
 public:
	MyMPU6050Class mpu;
	HMC5883L compass;
	
// === FUNKCJE ===
 public:
	void init();
	void readAngles(); // roll, pitch, yaw
	void readCompass();
	
	// te funkcje najlepiej wsadzic w program zeby oszczedzic na czasie
	float tiltCompensate(Vector mag, Vector normAccel);
	float noTiltCompensate(Vector mag);
	float correctAngle(float heading);
	
// === ZMIENNE ===
 public:
	struct ypr_angles
	{
		double pitch = 0;
		double roll = 0;
		float  yaw = 0;
		}angle;
	
	float headnigComp; // compensated heading / compass heading
	Vector magReading; // xyz kompasu
	Vector accScaled; // xyz acc (tylko dla obliczania kompasu)
	
	// Gyro variables
	uint32_t timeNow=0;
	uint32_t lastTime;
	float timeStep;
	float gyroYaw;
	float headnigGyroMagn; // gyro + magn headnig
	//float dpsPerDigit = .007633f; // dla skali 250DPS
	float dpsPerDigit = .060975f; // dla skali 2000DPS (niby ustawione 250 ale dziala dla 2000)
	
	volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
	
 private:
	// MPU control/status vars
	bool dmpReady = false;  // set true if DMP init was successful
	uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
	uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
	uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
	uint16_t fifoCount;     // count of all bytes currently in FIFO
	uint8_t fifoBuffer[64]; // FIFO storage buffer
	
	// orientation/motion vars
	Quaternion q;           // [w, x, y, z]         quaternion container
	VectorFloat gravity;    // [x, y, z]            gravity vector
	float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
};

extern SensorsClass sensors;

#endif

