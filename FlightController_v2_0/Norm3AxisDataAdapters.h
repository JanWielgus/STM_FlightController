// Norm3AxisDataAdapters.h

#ifndef _NORM3AXISDATAADAPTERS_h
#define _NORM3AXISDATAADAPTERS_h

#include "arduino.h"
#include "Interfaces.h"
#include "SharedDataTypes.h"

class NormAccelerometerAdapter : public INormalized3AxisDataAdapter
{
	vector3Int16& getNormalizedData() override;
};


class NormGyroscopeAdapter : public INormalized3AxisDataAdapter
{
	vector3Int16& getNormalizedData() override;
};


class NormMagnetometerAdapter : public INormalized3AxisDataAdapter
{
	vector3Int16& getNormalizedData() override;
};


#endif

