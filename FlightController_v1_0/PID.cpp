// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: t -*-

/// @file	PID.cpp
/// @brief	Generic PID algorithm

#include <math.h>
#include <Arduino.h>

#include "PID.h"

int32_t
PID::get_pid(float error)
{
	float output = 0;

	// Compute proportional component
	float pro = error * _kp;
	outPro = (int8_t)pro;
	output += pro;

    // Compute derivative component if time has elapsed
    if (dt_ms > 0) // NIE WIEM CZY POTRZEBNE BO I TAK TO SIE WYKONUJE STOSUNKOWO RZADKO !!!
	{
        float derivative;

		derivative = (error - _last_error) / dt_sec;

        // discrete low pass filter, cuts out the
        // high frequency noise that can drive the controller crazy
        //float RC = 1/(2*M_PI*_fCut);
        //derivative = _last_derivative + (delta_time / (RC + delta_time)) * (derivative - _last_derivative);

        // update state
        _last_error = error;
        _last_derivative = derivative;

        // add in derivative component
		float der = _kd * derivative;
		outDer = (int8_t)der;
        output += der;

    // Compute integral component
        _integrator += (error * _ki) * dt_sec;
		
		// anti wind-up
		_integrator = constrain(_integrator, -_imax, _imax);
		outInt = (int8_t)_integrator;
			
        output += _integrator;
    }

    return output;
}

void
PID::reset_I()
{
    _integrator = 0;
	// we use NAN (Not A Number) to indicate that the last 
	// derivative value is not valid
    _last_derivative = NAN;
}


uint32_t PID::dt_ms = 0;
float PID::dt_sec = 0;


void PID::updateDeltaTime()
{
	static uint32_t _last_t;
	static uint32_t tnow;
	tnow = millis();
	dt_ms = tnow - _last_t;
	_last_t = tnow;
	dt_sec = (float)dt_ms * 0.001;
}

