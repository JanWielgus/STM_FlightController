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

	void updateNonStaticParts(); // update fast and slow parts at the same time
	// or
	void updateFastParts(); // update fast-changing parts like throttle value
	void updateSlowParts(); // update slow-changing parts like PID value

private:
	//void print(const String& str, uint8_t col, uint8_t row);
	//void print(char symbol, uint8_t col, uint8_t row);
	template <class T>
	void print(T toPrint, uint8_t col, uint8_t row);

	char stickValueToSymbolHorizontal(int16_t value);
	char stickValueToSymbolVertical(int16_t value);
	void intToSizedString(String& outString, int16_t value, uint8_t length); // eg. value = 15, length = 4  ->  output: "  15"

	String fastShowLine; // string used for the fast show values with right alignment
};




#endif /* LCDHANDLER_H_ */