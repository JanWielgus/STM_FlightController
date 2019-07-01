/*
    Created:	01/07/2019
    Author:     Jan Wielgus
*/

#include <FC_Tasker.h>
#include <FC_CustomDataTypes.h>
#include <LiquidCrystal_I2C.h>
#include "ControlSticks.h"
#include "config.h"


FC_SimpleTasker tasker;

LiquidCrystal_I2C lcd(config::LCD_ADDRESS, 16, 2);



void setup()
{
	// Communication serial
	Serial.begin(9600);
	delay(300);
	
	// Add functions to the tasker
	tasker.addFunction(ctrlStk::readStickValues, 40000L, 15);
	tasker.scheduleTasks();


	ctrlStk::stickInitiazlize();
}


void loop()
{
	tasker.runTasker();
}
