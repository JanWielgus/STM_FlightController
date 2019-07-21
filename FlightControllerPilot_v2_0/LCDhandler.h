/*
 * LCDhandler.h
 *
 * Created: 20/07/2019 19:18:20
 *  Author: Jan Wielgus
 */ 


#include "Storage.h"



#ifndef LCDHANDLER_H_
#define LCDHANDLER_H_


namespace lcdH // LCD handler
{	
	void initLCD()
	{
		lcd.init(); // Wire.begin() is here
		lcd.backlight();
		lcd.setCursor(0, 0);
		lcd.print("FC Pilot");
		lcd.setCursor(0, 1);
		lcd.print("v 2.0");
		delay(700);
		lcd.clear();
	}
	
	
	void updateLCD()
	{
		// Print the throttle value
		lcd.clear();
		//lcd.setCursor(0, 0);
		lcd.print("Thr: ");
		lcd.print(thrStick.getValue());
		
		// print the state
		lcd.setCursor(0, 1);
		lcd.print("state: ");
		switch (state)
		{
			case disarmed:
			lcd.print("disarmed");
			break;
			case arming1:
			lcd.print("arming1");
			break;
			case arming2:
			lcd.print("arming2");
			break;
			case armed:
			lcd.print("armed");
			break;
		}
		
		lcd.setCursor(12, 0);
		lcd.print(com.connectionStability());
	}
}



#endif /* LCDHANDLER_H_ */