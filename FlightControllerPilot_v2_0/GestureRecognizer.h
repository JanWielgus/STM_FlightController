// GestureRecognizer.h

#ifndef _GESTURERECOGNIZER_h
#define _GESTURERECOGNIZER_h

#include <Arduino.h>




/////////// -----------------

// ---
// THIS CLASS HAVE TO BE REBUILT !!!!!!
// ---

//////////// -----------------



class GestureRecognizer
{
private:
	int16_t thr;
	int16_t rot;
	int16_t tb;
	int16_t lr;

public:
	GestureRecognizer();
	void recognizeArmingAndDisarmingGesture();
};


#endif

