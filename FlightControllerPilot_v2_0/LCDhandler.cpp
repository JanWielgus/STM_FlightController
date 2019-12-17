#include "LCDhandler.h"
#include "Storage.h"




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
	delay(700);
	lcd.clear();

	showStaticParts();
}


void LcdHandler::showStaticParts()
{
	lcd.clear();

	lcd.setCursor(0, 0);
	lcd.print("Thr: ");

	lcd.setCursor(0, 1);
	lcd.print("State: ");
}


void LcdHandler::updateLCD_nonStaticParts()
{
	updateLCD_fastParts();
	updateLCD_slowParts();
}


void LcdHandler::updateLCD_fastParts()
{
	String toShow;

	toShow = String(thrStick.getValue());
	while (toShow.length() < 4)
		toShow = " " + toShow;
	print(toShow, 4, 0);
}


void LcdHandler::updateLCD_slowParts()
{
	// print the state
	lcd.setCursor(7, 1);
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

	lcd.setCursor(10, 0);
	//lcd.print(com.connectionStability());
	//lcd.print(temp_counter);
	//lcd.print(MesasureTime::duration());
	lcd.print(btPID_P);
}


void LcdHandler::print(const String& str, uint8_t col, uint8_t row)
{
	lcd.setCursor(col, row);
	lcd.print(str);
}
