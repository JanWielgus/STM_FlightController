// AHRS.h

#ifndef _AHRS_h
#define _AHRS_h

#include "arduino.h"
#include "AHRS_Method.h"
#include "Interfaces.h"
#include <LowPassFilter.h> // in MyPID library files
#include "SharedDataTypes.h"
#include <FC_Task.h>


class AHRS : public FC_Task
{
private:
	struct ThreeAxisLowPassFilter
	{
		LowPassFilter x;
		LowPassFilter y;
		LowPassFilter z;
	};


public:
	AHRS(AHRS_Method* ahrsMethod, INormalized3AxisDataAdapter* accelerometer,
		INormalized3AxisDataAdapter* gyroscope);
	AHRS(AHRS_Method* ahrsMethod, INormalized3AxisDataAdapter* accelerometer,
		INormalized3AxisDataAdapter* gyroscope, INormalized3AxisDataAdapter* magnetometer);
	void execute() override;
	vector3Float& getAngles();
	float getPitch();
	float getRoll();
	float getHeading();


private:
	static void setUp3AxisLowPassFilter(ThreeAxisLowPassFilter* filterPtr, float cutOffFreq);
	static void update3AxisLPFonVector(ThreeAxisLowPassFilter* lowPassFilter, const vector3Float& sourceData, vector3Float* outputVector);


private:
	AHRS_Method* ahrsMethod;
	INormalized3AxisDataAdapter* acc;
	INormalized3AxisDataAdapter* gyro;
	INormalized3AxisDataAdapter* mag;


	// Accelerometer and gyroscope low-pass filters
	ThreeAxisLowPassFilter accLowPassFilter;
	ThreeAxisLowPassFilter gyroLowPassFilter;


	vector3Float accNormFiltered;
	vector3Float gyroNormFiltered;

	vector3Float* ahrsReturnVector;
	float pitch;
	float roll;
	float heading;
};


#endif

