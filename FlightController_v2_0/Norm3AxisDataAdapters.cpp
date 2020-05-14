#include "Norm3AxisDataAdapters.h"
#include "Storage.h"



vector3Float& NormAccelerometerAdapter::getNormalizedData()
{
	return Storage::rawMPU6050.getNormalizedAcceleration();
}



vector3Float& NormGyroscopeAdapter::getNormalizedData()
{
	return Storage::rawMPU6050.getNormalizedRotation();
}



vector3Float& NormMagnetometerAdapter::getNormalizedData()
{
	return Storage::rawHMC5883L.getNormalized();
}
