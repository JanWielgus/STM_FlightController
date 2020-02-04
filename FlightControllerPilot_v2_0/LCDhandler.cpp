#include "LCDhandler.h"
#include "Storage.h"
#include "config.h"




LcdHandler::LcdHandler()
{
	// do nothing
}


void LcdHandler::initLCD()
{
	lcd.init(); // Wire.begin() is here
	lcd.backlight();
	lcd.setCursor(0, 0);
	lcd.print("FC Pilot");
	lcd.setCursor(0, 1);
	lcd.print("v 2.0");
	delay(400);
	lcd.clear();

	showStaticParts();
}


void LcdHandler::showStaticParts()
{
	lcd.clear();

	lcd.setCursor(0, 1);
	lcd.print("S:");
}


void LcdHandler::updateNonStaticParts()
{
	updateFastParts();
	updateSlowParts();
}


void LcdHandler::updateFastParts()
{
	// Throttle
	intToSizedString(fastShowLine, thrStick.getValue(), 4);
	print(fastShowLine, 0, 0);

	// Other sticks
	lcd.setCursor(4, 0);
	lcd.print(stickValueToSymbolHorizontal(rotStick.getValue()));
	lcd.print(stickValueToSymbolVertical(TB_Stick.getValue()));
	lcd.print(stickValueToSymbolHorizontal(LR_Stick.getValue()));
	
	// Altitude
	print(com.received.altitude, 8, 0);
}


void LcdHandler::updateSlowParts()
{
	// print the state
	lcd.setCursor(2, 1);
	switch (armState)
	{
	case disarmed:
		lcd.print("dsmd");
		break;
	case arming1:
		lcd.print("a1");
		break;
	case arming2:
		lcd.print("a2");
		break;
	case armed:
		lcd.print("arm");
		break;
	}


	// Other slow parts

	// Drone angle
	print(com.received.tilt_TB, 8, 0);

	// P PID value from the android
	print(androidData.PID_P, 12, 0);

	// Right switch
	bool swState = digitalRead(config::pin.rightSwitch);
	print(swState, 15, 1);
}

/*
void LcdHandler::print(const String& str, uint8_t col, uint8_t row)
{
	lcd.setCursor(col, row);
	lcd.print(str);
}


void LcdHandler::print(char symbol, uint8_t col, uint8_t row)
{
	lcd.setCursor(col, row);
	lcd.print
}*/


template <class T>
void LcdHandler::print(T toPrint, uint8_t col, uint8_t row)
{
	lcd.setCursor(col, row);
	lcd.print(toPrint);
}


char LcdHandler::stickValueToSymbolHorizontal(int16_t value)
{
	if (value > 0)
		return '>';
	if (value < 0)
		return '<';
	return '|';
}


char LcdHandler::stickValueToSymbolVertical(int16_t value)
{
	if (value > 0)
		return '^';
	if (value < 0)
		return 'v';
	return '-';
}


// Int to right aligned string
void LcdHandler::intToSizedString(String& outString, int16_t value, uint8_t length)
{
	outString = String(value);
	while ((outString).length() < length)
		(outString) = " " + (outString);
}
