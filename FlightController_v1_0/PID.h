// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: t -*-

/// @file	PID.h
/// @brief	Generic PID algorithm, with EEPROM-backed storage of constants.

#ifndef PID_h
#define PID_h

//#include <AP_Common.h>
#include <math.h>               // for fabs()

/// @class	PID
/// @brief	Object managing one PID control
class PID
{
// Statyczne zmienne i metody
private:
	static uint32_t dt_ms;  // [ms]
	static float dt_sec;    // [s]
	
public:
	static void updateDeltaTime(); // Aktualizacja dt dla wszystkich obiektow klasy PID
	static float getDeltaTime() { return dt_sec; }
	
// Reszta kodu
public:

    PID(const float &   initial_p = 0.0,
        const float &   initial_i = 0.0,
        const float &   initial_d = 0.0,
        const int16_t & initial_imax = 0)
    {
        _kp = initial_p;
        _ki = initial_i;
        _kd = initial_d;
        _imax = initial_imax;

		// set _last_derivative as invalid when we startup
		_last_derivative = NAN;
    }

    /// Iterate the PID, return the new control value
    ///
    /// Positive error produces positive output.
    ///
    /// @param error	The measured error value
    /// @param dt		The time delta in milliseconds (note
    ///					that update interval cannot be more
    ///					than 65.535 seconds due to limited range
    ///					of the data type).
    /// @param scaler	An arbitrary scale factor
    ///
    /// @returns		The updated control output.
    ///
    int32_t        get_pid(float error);

    /// Reset the PID integrator
    ///
    void        reset_I();

    /// @name	parameter accessors
    //@{

/*
    float        kP() const {
        return _kp;
    }
    float        kI() const {
        return _ki;
    }
    float        kD() const {
        return _kd;
    }
    int16_t        imax() const {
        return _imax;
    }*/

    void kP(const float v)
	{
        _kp=v;
    }
    void kI(const float v)
	{
        _ki=v;
    }
    void kD(const float v)
	{
        _kd=v;
    }
    void imax(const int16_t v)
	{
        _imax=abs(v);
    }
/*
    float        get_integrator() const
	{
        return _integrator;
    }
	*/

private:
    float _kp;
    float _ki;
    float _kd;
    int16_t _imax;

    float _integrator;                 ///< integrator value
    int32_t _last_error;               ///< last error for derivative
    float _last_derivative;            ///< last derivative for low-pass filter

    int32_t         _get_pid(int32_t error, uint16_t dt, float scaler);

    /// Low pass filter cut frequency for derivative calculation.
    ///
    /// 20 Hz becasue anything over that is probably noise, see
    /// http://en.wikipedia.org/wiki/Low-pass_filter.
    ///
    static const uint8_t        _fCut = 20;
	
public:
	// zmienne do rysowania wykresow
	int8_t outPro; // P
	int8_t outInt; // I
	int8_t outDer; // D
};

#endif
