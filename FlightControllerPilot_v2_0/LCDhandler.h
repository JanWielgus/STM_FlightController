/*
 * LCDhandler.h
 *
 * Created: 20/07/2019 19:18:20
 *  Author: Jan Wielgus
 */ 


#ifndef LCDHANDLER_H_
#define LCDHANDLER_H_

#include <Arduino.h>


////////////////


// I DON'T KNOW WHY THIS IS WORKING !!!
// THERE SHOULD BE A CPP FILE WITH VARIABLE DECLARATION
// IN H FILE SHOULD BE ONLY EXTERN OR NOTHING IF IT IS NOT USED !!!!
// !!!!
// !!!

// THE SAME IS IN EVERY H FILE WITHOUT CPP FILE !!!

//////////////




class LcdHandler
{
public:
	LcdHandler();
	void initLCD();
	void showStaticParts(); // print on the LCD parts that do not change over time (like "Thr:"

	void updateLCD_nonStaticParts(); // update fast and slow parts at the same time
	// or
	void updateLCD_fastParts(); // update fast-changing parts like throttle value
	void updateLCD_slowParts();

private:
	void print(const String& str, uint8_t col, uint8_t row);
};



#endif /* LCDHANDLER_H_ */