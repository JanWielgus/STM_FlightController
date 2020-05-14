// 
// 
// 

#include "AHRS.h"
#include "config.h"
#include "Storage.h"


AHRS::AHRS(AHRS_Method* ahrsMethod, INormalized3AxisDataAdapter* accelerometer, INormalized3AxisDataAdapter* gyroscope)
{
	this->ahrsMethod = ahrsMethod;
	this->acc = accelerometer;
	this->gyro = gyroscope;
	this->mag = nullptr;

	// Set up low-pass filters
	setUp3AxisLowPassFilter(&accLowPassFilter, config::sensorCalibration.accLowPassFilterCutOffFreq);
	setUp3AxisLowPassFilter(&gyroLowPassFilter, config::sensorCalibration.gyroLowPassFilterCutOffFreq);
}


AHRS::AHRS(AHRS_Method* ahrsMethod, INormalized3AxisDataAdapter* accelerometer, INormalized3AxisDataAdapter* gyroscope, INormalized3AxisDataAdapter* magnetometer)
{
	this->ahrsMethod = ahrsMethod;
	this->acc = accelerometer;
	this->gyro = gyroscope;
	this->mag = magnetometer;

	// Set up low-pass filters
	setUp3AxisLowPassFilter(&accLowPassFilter, config::sensorCalibration.accLowPassFilterCutOffFreq);
	setUp3AxisLowPassFilter(&gyroLowPassFilter, config::sensorCalibration.gyroLowPassFilterCutOffFreq);
}


void AHRS::execute()
{
	update3AxisLPFonVector(&accLowPassFilter, acc->getNormalizedData(), &accNormFiltered);
	update3AxisLPFonVector(&gyroLowPassFilter, gyro->getNormalizedData(), &gyroNormFiltered);

	if (mag != nullptr)
		ahrsMethod->setData(accNormFiltered, gyroNormFiltered, mag->getNormalizedData());
	else
		ahrsMethod->setData(accNormFiltered, gyroNormFiltered);
	
	// Run main calculations
	ahrsReturnVector = ahrsMethod->update();

	// update pitch, roll and heading
	pitch = ahrsReturnVector->x;
	roll = ahrsReturnVector->y;
	heading = ahrsReturnVector->z;

	// Update values in storage for fast use
	Storage::reading.pitch = pitch;
	Storage::reading.roll = roll;
	Storage::reading.heading = heading;
}


vector3Float& AHRS::getAngles()
{
	return *(ahrsMethod->getAngles());
}


float AHRS::getPitch()
{
	return pitch;
}


float AHRS::getRoll()
{
	return roll;
}


float AHRS::getHeading()
{
	return heading;
}


void AHRS::setUp3AxisLowPassFilter(ThreeAxisLowPassFilter* filterPtr, float cutOffFreq)
{
	filterPtr->x.reconfigureFilter(config::MainDeltaTimeInSeconds, cutOffFreq);
	filterPtr->y.reconfigureFilter(config::MainDeltaTimeInSeconds, cutOffFreq);
	filterPtr->z.reconfigureFilter(config::MainDeltaTimeInSeconds, cutOffFreq);
}


void AHRS::update3AxisLPFonVector(ThreeAxisLowPassFilter* lowPassFilter, const vector3Float& sourceData, vector3Float* outputVector)
{
	outputVector->x = lowPassFilter->x.update(sourceData.x);
	outputVector->y = lowPassFilter->y.update(sourceData.y);
	outputVector->z = lowPassFilter->z.update(sourceData.z);
}


