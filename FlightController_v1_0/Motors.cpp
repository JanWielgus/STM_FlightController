// Author: Jan Wielgus
// Date: 25.10.2017
// 

#include "Motors.h"
MotorsClass motors;



void MotorsClass::init()
{
	mTL.attach(TLmotorPin); // Top left
	mTR.attach(TRmotorPin); // Top right
	mBL.attach(BLmotorPin); // Back left
	mBR.attach(BRmotorPin); // Back right
	setOnAllMotors(0);      // idle
}



void MotorsClass::setMotors(bool _state)
{
	armStateFlag = _state;
}



void MotorsClass::setOnAllMotors(int16_t _val)
{
	if (armStateFlag)
	{
		_val += 1000;
		_val = constrain(_val, MOTOR_MIN, MOTOR_MAX);
		
		mTL.writeMicroseconds(_val);
		mTR.writeMicroseconds(_val);
		mBL.writeMicroseconds(_val);
		mBR.writeMicroseconds(_val);
	}
	else
	{
		mTL.writeMicroseconds(MOTOR_IDLE);
		mTR.writeMicroseconds(MOTOR_IDLE);
		mBL.writeMicroseconds(MOTOR_IDLE);
		mBR.writeMicroseconds(MOTOR_IDLE);
	}
}



void MotorsClass::setOnTL(int16_t _val)
{
	if (armStateFlag)
	{
		_val += 1000;
		_val = constrain(_val, MOTOR_MIN, MOTOR_MAX);
		mTL.writeMicroseconds(_val);
	}
	else
		mTL.writeMicroseconds(MOTOR_IDLE);
}



void MotorsClass::setOnTR(int16_t _val)
{
	if (armStateFlag)
	{
		_val += 1000;
		_val = constrain(_val, MOTOR_MIN, MOTOR_MAX);
		mTR.writeMicroseconds(_val);
	}
	else
		mTR.writeMicroseconds(MOTOR_IDLE);
}



void MotorsClass::setOnBL(int16_t _val)
{
	if (armStateFlag)
	{
		_val += 1000;
		_val = constrain(_val, MOTOR_MIN, MOTOR_MAX);
		mBL.writeMicroseconds(_val);
	}
	else
		mBL.writeMicroseconds(MOTOR_IDLE);
}



void MotorsClass::setOnBR(int16_t _val)
{
	if (armStateFlag)
	{
		_val += 1000;
		_val = constrain(_val, MOTOR_MIN, MOTOR_MAX);
		mBR.writeMicroseconds(_val);
	}
	else
		mBR.writeMicroseconds(MOTOR_IDLE);
}



