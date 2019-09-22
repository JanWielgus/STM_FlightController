/*
 * LCDhandler.h
 *
 * Created: 20/07/2019 19:18:20
 *  Author: Jan Wielgus
 */ 


#ifndef LCDHANDLER_H_
#define LCDHANDLER_H_

#include "Storage.h"



class MesasureTime
{
	public:
	
	static uint32_t startTime;
	static uint32_t dur;
	
	public:
	static void start()
	{
		startTime = micros();
	}
	
	static void end()
	{
		dur = micros() - startTime;
	}
	
	static uint16_t duration()
	{
		return dur;
	}
};
uint32_t MesasureTime::startTime = 0;
uint32_t MesasureTime::dur = 0;

uint16_t counter = 0; // Used to measure how many times function does in a second (checked in the updateControlDiode() )
uint16_t temp_counter = 0;




namespace lcdH // LCD handler
{
	void initLCD();
	void updateLCD();
	char stickToChar(int16_t value, bool horizontal=true);


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

		// Print sticks
		lcd.print(thrStick.getValue());
		lcd.setCursor(4, 0);
		lcd.print(stickToChar(rotStick.getValue()));
		lcd.print(stickToChar(TB_Stick.getValue(), false));
		lcd.print(stickToChar(LR_Stick.getValue()));

		// print drone angle
		lcd.setCursor(8, 0);
		lcd.print(com.received.tilt_TB);
		
		// print the state
		lcd.setCursor(0, 1);
		lcd.print("s:");
		switch (state)
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

		lcd.setCursor(7, 1);
		lcd.print(com.received.altitude);
		
		lcd.setCursor(12, 0);
		//lcd.print(com.connectionStability());
		//lcd.print(temp_counter);
		//lcd.print(MesasureTime::duration());
		lcd.print(btPID_P);

		lcd.setCursor(15, 1);
		lcd.print(digitalRead(config::pin.rightSwitch));
	}


	// Change stick value to the '\' or '|' or '/'
	char stickToChar(int16_t value, bool horizontal)
	{
		if (horizontal)
		{
			if (value > 0)
				return '>';
			if (value < 0)
				return '<';
			return '|';
		}
		else
		{
			if (value > 0)
				return '^';
			if (value < 0)
				return 'v';
			return '-';
		}
	}
}



#endif /* LCDHANDLER_H_ */