// AHRS_Method.h

#ifndef _AHRS_METHOD_h
#define _AHRS_METHOD_h

#include "arduino.h"
#include "SharedDataTypes.h"
#include "math.h"


// Abstract class for all AHRS methods
class AHRS_Method
{
public:
	AHRS_Method();
	void setData(const vector3Float& acc, const vector3Float& gyro);
	void setData(const vector3Float& acc, const vector3Float& gyro, const vector3Float& mag);
	vector3Float* getAngles();
	virtual vector3Float* update() = 0;

protected:
	int16_t ax, ay, az;
	int16_t gx, gy, gz;
	int16_t mx, my, mz;
	vector3Float angles; // x-pitch, y-roll, z-yaw
};





// Abstract class, base for two types of Madgwick AHRS
class MadgwickBase
{
public:
	MadgwickBase(float sampleFrequency, float beta = DefaultBeta);
	static float invSqrt(float x);

protected:
	static const float DefaultBeta;
	float beta; // algorithm gain (2 * proportional gain)

	float q0;
	float q1;
	float q2;
	float q3; // quaternion of sensor frame relative to auxiliary frame
	float invSampleFreq;

	float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
};




// Full Madgwick AHRS algorithm
class MadgwickAHRS : public AHRS_Method, MadgwickBase
{
public:
	MadgwickAHRS(float sampleFrequency, float beta = DefaultBeta);
	vector3Float* update() override;

protected:
	float hx, hy;
	float _2q0mx, _2q0my, _2q0mz, _2q1mx,
		_2bx, _2bz, _4bx, _4bz,
		_2q0, _2q1, _2q2, _2q3,
		_2q0q2, _2q2q3,
		q0q0, q0q1, q0q2, q0q3,
		q1q1, q1q2, q1q3,
		q2q2, q2q3, q3q3;
};




// Madgwick IMU algorithm (without compass)
class MadgwickIMU : public AHRS_Method, MadgwickBase
{
public:
	MadgwickIMU(float sampleFrequency, float beta = DefaultBeta);
	vector3Float* update() override;

protected:
	float _2q0, _2q1, _2q2, _2q3,
		_4q0, _4q1, _4q2,
		_8q1, _8q2,
		q0q0, q1q1, q2q2, q3q3;
};





/*
// Simple IMU based on JoopBrokking
class SimpleIMU : public AHRS_Method
{
public:
	SimpleIMU();
	vector3Float* update() override;

protected:
	// necessary data
};*/




#endif

