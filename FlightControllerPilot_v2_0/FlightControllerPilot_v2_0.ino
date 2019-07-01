/*
    Created:	01/07/2019
    Author:     Jan Wielgus
*/

#include <FC_Tasker.h>
#include <FC_CustomDataTypes.h>
#include <LiquidCrystal_I2C.h>
#include "FC_MainCommunication.h"
#include "FC_ControlStick.h"
#include "config.h"


FC_SimpleTasker tasker;

LiquidCrystal_I2C lcd(config::LCD_ADDRESS, 16, 2);



void setup()
{
	// Communication serial
	Serial.begin(9600);
	delay(300);
	
	// Add functions to the tasker
	//tasker.addFunction(ctrlStk::readStickValues, 40000L, 15);
	tasker.scheduleTasks();

	
	lcd.init(); // Wire.begin() is here
	lcd.backlight();
	lcd.setCursor(0, 0);
	lcd.print("FC Pilot");
	lcd.setCursor(0, 1);
	lcd.print("v 2.0");
	delay(700);
	lcd.clear();
}


void loop()
{
	tasker.runTasker();
}
