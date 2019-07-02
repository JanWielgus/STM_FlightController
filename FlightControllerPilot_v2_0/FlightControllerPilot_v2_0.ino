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


// control sticks
FC_ControlStick thrStick;
FC_ControlStick rotStick;
FC_ControlStick TB_Stick;
FC_ControlStick LR_Stick;


// Tasker function prototypes
void readControlSticksValues();
void updateLCD();
void tempSerial();


enum stateType {disarmed, armed};
stateType state = disarmed;



void setup()
{
	// Communication serial
	Serial.begin(115200);
	delay(300);
	
	// Add functions to the tasker
	tasker.addFunction(readControlSticksValues, 20000L, 15); // 50Hz
	tasker.addFunction(updateLCD, 41666L, 15); // 24Hz
	tasker.addFunction(tempSerial, 40000L, 15); //25Hz
	tasker.scheduleTasks();
	
	
	// init the control sticks
	thrStick.setInputProperties(config::pin.throttle, true, config::tiltsRange.thrMin, config::tiltsRange.thrMax);
	rotStick.setInputProperties(config::pin.rotate, true, config::tiltsRange.rotMin, config::tiltsRange.rotMax);
	TB_Stick.setInputProperties(config::pin.tiltTB, true, config::tiltsRange.TB_Min, config::tiltsRange.TB_Max);
	LR_Stick.setInputProperties(config::pin.tiltLR, true, config::tiltsRange.LR_Min, config::tiltsRange.LR_Max);
	thrStick.setOutputValueProperties(0, 1000, config::tiltsRange.thrCen, config::stickDeadZone);
	rotStick.setOutputValueProperties(-500, 500, config::tiltsRange.rotCen, config::stickDeadZone);
	TB_Stick.setOutputValueProperties(-500, 500, config::tiltsRange.TB_Cen, config::stickDeadZone);
	LR_Stick.setOutputValueProperties(-500, 500, config::tiltsRange.LR_Cen, config::stickDeadZone);

	
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


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



void readControlSticksValues()
{
	thrStick.readValue();
	rotStick.readValue();
	TB_Stick.readValue();
	LR_Stick.readValue();
}


void updateLCD()
{
	// Print the throttle value
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Thr: ");
	lcd.print(thrStick.getValue());
	
	// print the state
	lcd.setCursor(0, 1);
	lcd.print("state: ");
	switch (state)
	{
		case armed:
			lcd.print("armed");
			break;
		case disarmed:
			lcd.print("disarmed");
			break;
	}
}


void tempSerial()
{
	Serial.println(TB_Stick.getValue());
}
