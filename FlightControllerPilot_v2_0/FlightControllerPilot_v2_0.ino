/*
    Created:	01/07/2019
    Author:     Jan Wielgus
*/

#include "Storage.h"
#include <FC_CustomDataTypes.h>
#include "config.h"
#include "GestureRecognition.h"
#include "LCDhandler.h"



// Tasker function prototypes
void updateMainCommunication();
void readControlSticksValues();
void gestureRecognition();
void updateControlDiode();



void setup()
{
	// Communication serial
	Serial.begin(19200);
	delay(300);
	
	pinMode(LED_BUILTIN, OUTPUT);
	
	
	// TEMPORARY !!!  for the radio module
	pinMode(config::pin.m0pin, OUTPUT);
	pinMode(config::pin.m1pin, OUTPUT);
	pinMode(config::pin.aux, INPUT);
	digitalWrite(config::pin.m0pin, LOW);
	digitalWrite(config::pin.m1pin, LOW);
	
	// Add functions to the tasker
	tasker.addFunction(updateMainCommunication, 40000L, 11);
	tasker.addFunction(readControlSticksValues, 20000L, 14); // 50Hz
	tasker.addFunction(lcdH::updateLCD, 100000L, 10); // 10Hz
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

	
	lcdH::initLCD();
	
	// Necessary for LCD to keep up
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
	//com.packAndSendData(com.sendPacketTypes.TYPE1_ID, com.sendPacketTypes.TYPE1_SIZE);
}



void readControlSticksValues()
{
	thrStick.readValue();
	rotStick.readValue();
	TB_Stick.readValue();
	LR_Stick.readValue();
}


void gestureRecognition()
{		
	gr::recognizeArmingAndDisarmingGesture(thrStick.getValue(),
										   rotStick.getValue(),
										   TB_Stick.getValue(),
										   LR_Stick.getValue());
}

