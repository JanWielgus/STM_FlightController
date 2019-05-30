// Author: Jan Wielgus
// Date: 02.01.2018
//


#include "FC_Motors.h"


FC_Motors::FC_Motors()
{
	motorsState = false;
	
	TIMER3_BASE->CR1 = TIMER_CR1_CEN | TIMER_CR1_ARPE;
	TIMER3_BASE->CR2 = 0;
	TIMER3_BASE->SMCR = 0;
	TIMER3_BASE->DIER = 0;
	TIMER3_BASE->EGR = 0;
	TIMER3_BASE->CCMR1 = (0b110 << 4) | TIMER_CCMR1_OC1PE |(0b110 << 12) | TIMER_CCMR1_OC2PE;
	TIMER3_BASE->CCMR2 = (0b110 << 4) | TIMER_CCMR2_OC3PE |(0b110 << 12) | TIMER_CCMR2_OC4PE;
	TIMER3_BASE->CCER = TIMER_CCER_CC1E | TIMER_CCER_CC2E | TIMER_CCER_CC3E | TIMER_CCER_CC4E;
	TIMER3_BASE->PSC = 71;
	TIMER3_BASE->ARR = 5000;
	TIMER3_BASE->DCR = 0;

	TIMER3_BASE->CCR1 = 1000;
	TIMER3_BASE->CCR2 = 1000;
	TIMER3_BASE->CCR3 = 1000;
	TIMER3_BASE->CCR4 = 1000;
	
	pinMode(TLmotorPin, PWM);
	pinMode(TRmotorPin, PWM);
	pinMode(BLmotorPin, PWM);
	pinMode(BRmotorPin, PWM);
	
	// Set all on idle
	setOnTL(IDLE);
	setOnTR(IDLE);
	setOnBL(IDLE);
	setOnBR(IDLE);
}


void FC_Motors::setMotorState(bool state)
{
	if (this->motorsState == true && state == false) // if motors will be disabled
	{
		// Set all on idle
		setOnTL(IDLE);
		setOnTR(IDLE);
		setOnBL(IDLE);
		setOnBR(IDLE);
	}
	
	this->motorsState = state;
}


void FC_Motors::setOnTL(int16_t val)
{
	if (motorsState)
	{
		val += MotorsDispVal;
		val = constrain(val, MotorMin, MotorMax);
		TIMER3_BASE->CCR1 = val; // TL
	}
	else
		TIMER3_BASE->CCR1 = MotorIdle;
}


void FC_Motors::setOnTR(int16_t val)
{
	if (motorsState)
	{
		val += MotorsDispVal;
		val = constrain(val, MotorMin, MotorMax);
		TIMER3_BASE->CCR3 = val; // TR
	}
	else
		TIMER3_BASE->CCR3 = MotorIdle;
}


void FC_Motors::setOnBL(int16_t val)
{
	if (motorsState)
	{
		val += MotorsDispVal;
		val = constrain(val, MotorMin, MotorMax);
		TIMER3_BASE->CCR2 = val; // BL
	}
	else
		TIMER3_BASE->CCR2 = MotorIdle;
}


void FC_Motors::setOnBR(int16_t val)
{
	if (motorsState)
	{
		val += MotorsDispVal;
		val = constrain(val, MotorMin, MotorMax);
		TIMER3_BASE->CCR4 = val;
	}
	else
		TIMER3_BASE->CCR4 = MotorIdle; // BR
}


void FC_Motors::forceMotorsExecution()
{
	TIMER3_BASE->CNT = 5000; // This will reset Timer3 and cause creating pulses
}
