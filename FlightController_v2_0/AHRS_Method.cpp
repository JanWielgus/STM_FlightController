// 
// 
// 

#include "AHRS_Method.h"


AHRS_Method::AHRS_Method()
{
	ax = ay = az = 0;
	gx = gy = gz = 0;
	mx = my = mz = 0;
	angles = { 0, 0, 0 };
}


void AHRS_Method::setData(const vector3Float& acc, const vector3Float& gyro)
{
	// Acc
	ax = acc.x;
	ay = acc.y;
	az = acc.z;

	// Gyro
	gx = gyro.x;
	gy = gyro.y;
	gz = gyro.z;
}


void AHRS_Method::setData(const vector3Float& acc, const vector3Float& gyro, const vector3Float& mag)
{
	// Acc
	ax = acc.x;
	ay = acc.y;
	az = acc.z;

	// Gyro
	gx = gyro.x;
	gy = gyro.y;
	gz = gyro.z;

	// Mag
	mx = mag.x;
	my = mag.y;
	mz = mag.z;
}


vector3Float* AHRS_Method::getAngles()
{
	return &angles;
}
