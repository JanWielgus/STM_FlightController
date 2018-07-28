// Author: Jan Wielgus
// Date: 25.10.2017
// 

#include "Motors.h"
MotorsClass motors;



void MotorsClass::init()
{
	TIMER4_BASE->CR1 = TIMER_CR1_CEN | TIMER_CR1_ARPE;
	TIMER4_BASE->CR2 = 0;
	TIMER4_BASE->SMCR = 0;
	TIMER4_BASE->DIER = 0;
	TIMER4_BASE->EGR = 0;
	TIMER4_BASE->CCMR1 = (0b110 << 4) | TIMER_CCMR1_OC1PE |(0b110 << 12) | TIMER_CCMR1_OC2PE;
	TIMER4_BASE->CCMR2 = (0b110 << 4) | TIMER_CCMR2_OC3PE |(0b110 << 12) | TIMER_CCMR2_OC4PE;
	TIMER4_BASE->CCER = TIMER_CCER_CC1E | TIMER_CCER_CC2E | TIMER_CCER_CC3E | TIMER_CCER_CC4E;
	TIMER4_BASE->PSC = 71;
	TIMER4_BASE->ARR = 5000;
	TIMER4_BASE->DCR = 0;
	TIMER4_BASE->CCR1 = 1000;

	TIMER4_BASE->CCR1 = 1000;
	TIMER4_BASE->CCR2 = 1000;
	TIMER4_BASE->CCR3 = 1000;
	TIMER4_BASE->CCR4 = 1000;
	
	pinMode(TLmotorPin, PWM);
	pinMode(TRmotorPin, PWM);
	pinMode(BLmotorPin, PWM);
	pinMode(BRmotorPin, PWM);
	
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
		
		TIMER4_BASE->CCR1 = _val; // TL
		TIMER4_BASE->CCR2 = _val; // BL
		TIMER4_BASE->CCR3 = _val; // TR
		TIMER4_BASE->CCR4 = _val; // BR
	}
	else
	{
		TIMER4_BASE->CCR1 = MOTOR_IDLE; // TL
		TIMER4_BASE->CCR2 = MOTOR_IDLE; // BL
		TIMER4_BASE->CCR3 = MOTOR_IDLE; // TR
		TIMER4_BASE->CCR4 = MOTOR_IDLE; // BR
	}
	
	TIMER4_BASE->CNT = 5000; // This will reset timer 4 and cause creating pulses
}



void MotorsClass::setOnTL(int16_t _val)
{
	if (armStateFlag)
	{
		_val += 1000;
		_val = constrain(_val, MOTOR_MIN, MOTOR_MAX);
		TIMER4_BASE->CCR1 = _val; // TL
	}
	else
		TIMER4_BASE->CCR1 = MOTOR_IDLE; // TL
}



void MotorsClass::setOnTR(int16_t _val)
{
	if (armStateFlag)
	{
		_val += 1000;
		_val = constrain(_val, MOTOR_MIN, MOTOR_MAX);
		TIMER4_BASE->CCR3 = _val; // TR
	}
	else
		TIMER4_BASE->CCR3 = MOTOR_IDLE; // TR
}



void MotorsClass::setOnBL(int16_t _val)
{
	if (armStateFlag)
	{
		_val += 1000;
		_val = constrain(_val, MOTOR_MIN, MOTOR_MAX);
		TIMER4_BASE->CCR2 = _val; // BL
	}
	else
		TIMER4_BASE->CCR2 = MOTOR_IDLE; // BL
}



void MotorsClass::setOnBR(int16_t _val)
{
	if (armStateFlag)
	{
		_val += 1000;
		_val = constrain(_val, MOTOR_MIN, MOTOR_MAX);
		TIMER4_BASE->CCR4 = _val; // BR
	}
	else
		TIMER4_BASE->CCR4 = MOTOR_IDLE; // BR
}



void MotorsClass::executeMotorsNow()
{
	TIMER4_BASE->CNT = 5000; // This will reset timer 4 and cause creating pulses
}



