// AHRS_Method.h

#ifndef _AHRS_METHOD_h
#define _AHRS_METHOD_h

#include "arduino.h"
#include "SharedDataTypes.h"


class AHRS_Method
{
public:
	AHRS_Method();
	void setData(const vector3Float& acc, const vector3Float& gyro);
	void setData(const vector3Float& acc, const vector3Float& gyro, const vector3Float& mag);
	vector3Float* getAngles();
	virtual vector3Float* update() = 0;



private:
	int16_t ax, ay, az;
	int16_t gx, gy, gz;
	int16_t mx, my, mz;
	vector3Float angles; // x-pitch, y-roll, z-yaw
};


#endif

