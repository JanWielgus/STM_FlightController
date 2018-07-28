// Motors.h
// Author: Jan Wielgus
// Date: 25.10.2017

#ifndef _MOTORS_h
#define _MOTORS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "config.h"

// Stany silnikow
#define ARMED 1
#define IDLE 0


class MotorsClass
{
 public:
	
// === FUNKCJE ===
 public:
	void init();                        // inicjalizacja
	void setOnAllMotors(int16_t _val);  // parametr od 0 do 1000
	void setMotors(bool _state);        // Uzbrajanie/rozzbrajanie silników (true - arm, false - disarm)
	
	// Ustawianie na odpowiednie silniki
	void setOnTL(int16_t _val);    // parametr od 0 do 1000
	void setOnTR(int16_t _val);    // parametr od 0 do 1000
	void setOnBL(int16_t _val);    // parametr od 0 do 1000
	void setOnBR(int16_t _val);    // parametr od 0 do 1000
	void executeMotorsNow();       // Reset timer 4 and generate pulses. IF THIS is NOT used (can be) cause 200Hz pulse
	
// === ZMIENNE ===
 public:
	
	
 private:
	bool armStateFlag = false;    // Zmienna stanu uzbrojenia silników
	
};

extern MotorsClass motors;

#endif

