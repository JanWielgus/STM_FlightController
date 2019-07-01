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
FC_ControlStick stickThr(config::pin.throttle, true, config::tiltsRange.thrMin, config::tiltsRange.thrMax);
FC_ControlStick stickRot(config::pin.rotate, true, config::tiltsRange.rotMin, config::tiltsRange.rotMax);
FC_ControlStick stickTB(config::pin.tiltTB, true, config::tiltsRange.TB_Min, config::tiltsRange.TB_Max);
FC_ControlStick stickLR(config::pin.tiltLR, true, config::tiltsRange.LR_Min, config::tiltsRange.LR_Max);


// Tasker function prototypes
void readControlSticksValues();
void tempSerial();



void setup()
{
	// Communication serial
	Serial.begin(115200);
	delay(300);
	
	// Add functions to the tasker
	tasker.addFunction(readControlSticksValues, 20000L, 15); // 50Hz
	tasker.addFunction(tempSerial, 40000L, 15); //25Hz;
	tasker.scheduleTasks();
	
	
	// init the control sticks
	stickThr.setOutputValueProperties(0, 1000, config::tiltsRange.thrCen, config::stickDeadZone);
	stickRot.setOutputValueProperties(-500, 500, config::tiltsRange.rotCen, config::stickDeadZone);
	stickTB.setOutputValueProperties(-500, 500, config::tiltsRange.TB_Cen, config::stickDeadZone);
	stickLR.setOutputValueProperties(-500, 500, config::tiltsRange.LR_Cen, config::stickDeadZone);

	
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



void readControlSticksValues()
{
	stickThr.readValue();
	stickRot.readValue();
	stickTB.readValue();
	stickLR.readValue();
}


void tempSerial()
{
	Serial.println(stickTB.getValue());
}
