#include "Norm3AxisDataAdapters.h"
#include "Storage.h"



vector3Int16& NormAccelerometerAdapter::getNormalizedData()
{
	return Storage::rawMPU6050.getRawAcceleration();
}



vector3Int16& NormGyroscopeAdapter::getNormalizedData()
{
	return Storage::rawMPU6050.getRawRotation();
}



vector3Int16& NormMagnetometerAdapter::getNormalizedData()
{
	return Storage::rawHMC5883L.getRaw();
}
