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
#include "GestureRecognition.h"


FC_SimpleTasker tasker;

FC_MainCommunication com(&Serial, 45);

LiquidCrystal_I2C lcd(config::LCD_ADDRESS, 16, 2);


// control sticks
FC_ControlStick thrStick;
FC_ControlStick rotStick;
FC_ControlStick TB_Stick;
FC_ControlStick LR_Stick;


// Tasker function prototypes
void updateMainCommunication();
void readControlSticksValues();
void updateLCD();
void gestureRecognition();
void updateControlDiode();


void setup()
{
	// Communication serial
	Serial.begin(9600);
	delay(300);
	
	pinMode(LED_BUILTIN, OUTPUT);
	
	// Add functions to the tasker
	tasker.addFunction(updateMainCommunication, 40000L, 11);
	tasker.addFunction(readControlSticksValues, 20000L, 14); // 50Hz
	tasker.addFunction(updateLCD, 100000L, 10); // 10Hz
	tasker.addFunction(gestureRecognition, 100001L, 16); // 10Hz (without 1 at the end is 7/8Hz)
	tasker.addFunction(updateControlDiode, 1000000L, 5); // blink builtin diode every second
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
	
	// Necessary
	Wire.setClock(400000L);
}


void loop()
{
	tasker.runTasker();
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void updateControlDiode()
{
	
	static bool ledState = LOW;
	digitalWrite(LED_BUILTIN, ledState);
	ledState = !ledState;
}



void updateMainCommunication()
{
	com.receiveAndUnpackData();
	
	// pack data
	com.toSend.steer.throttle = thrStick.getValue();
	com.toSend.steer.rotate = rotStick.getValue();
	com.toSend.steer.TB = TB_Stick.getValue();
	com.toSend.steer.LR = LR_Stick.getValue();
	com.toSend.arming = state==armed ? 1 : 0;
	// OTHER DATA !!!
	// send packed data
	com.packAndSendData(com.sendPacketTypes.TYPE2_ID, com.sendPacketTypes.TYPE2_SIZE);
}



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


void gestureRecognition()
{		
	gr::recognizeArmingAndDisarmingGesture(thrStick.getValue(),
										   rotStick.getValue(),
										   TB_Stick.getValue(),
										   LR_Stick.getValue());
}

