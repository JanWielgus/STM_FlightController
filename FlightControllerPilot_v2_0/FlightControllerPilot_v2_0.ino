/*
    Created:	01/07/2019
    Author:     Jan Wielgus
*/

#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <FC_Tasker.h>
#include <FC_Communication_Base.h>
#include <FC_CustomDataTypes.h>
#include <FC_EVA_Filter.h>
#include "Storage.h"
#include "config.h"
#include "LCDhandler.h"
#include "TaskerFunctions.h"
#include "AndroidCommunication.h"
#include "GestureRecognizer.h"



void setup()
{
	// Communication serial
	Serial.begin(19200);
	delay(300);
	
	// bluetooth software serial
	androidCom.init();
	
	pinMode(LED_BUILTIN, OUTPUT);
	
	//switches
	pinMode(config::pin.leftSwitch, INPUT_PULLUP);
	pinMode(config::pin.rightSwitch, INPUT_PULLUP);
	
	
	// TEMPORARY !!!  for the radio module
	pinMode(config::pin.m0pin, OUTPUT);
	pinMode(config::pin.m1pin, OUTPUT);
	pinMode(config::pin.aux, INPUT);
	// set full-duplex
	digitalWrite(config::pin.m0pin, HIGH);
	digitalWrite(config::pin.m1pin, LOW);
	


	addTaskerFunctionsToTasker();
	
	
	// init the control sticks
	thrStick.setInputProperties(config::pin.throttle, true, config::sticksCalibVal.thrMin, config::sticksCalibVal.thrMax);
	rotStick.setInputProperties(config::pin.rotate, true, config::sticksCalibVal.rotMin, config::sticksCalibVal.rotMax);
	TB_Stick.setInputProperties(config::pin.tiltTB, true, config::sticksCalibVal.TB_Min, config::sticksCalibVal.TB_Max);
	LR_Stick.setInputProperties(config::pin.tiltLR, true, config::sticksCalibVal.LR_Min, config::sticksCalibVal.LR_Max);
	thrStick.setOutputValueProperties(0, 1000, config::sticksCalibVal.thrCen, config::stickDeadZone);
	rotStick.setOutputValueProperties(-500, 500, config::sticksCalibVal.rotCen, config::stickDeadZone);
	TB_Stick.setOutputValueProperties(-500, 500, config::sticksCalibVal.TB_Cen, config::stickDeadZone);
	LR_Stick.setOutputValueProperties(-500, 500, config::sticksCalibVal.LR_Cen, config::stickDeadZone);
	
	// set stick filtering
	thrStick.setFilterIntensity(60);
	rotStick.setFilterIntensity(60);
	TB_Stick.setFilterIntensity(60);
	LR_Stick.setFilterIntensity(60);

	
	// display
	display.initLCD();
	display.showStaticParts();

	
	// Necessary for LCD to keep up
	Wire.setClock(400000L);
}


void loop()
{
	androidCom.read(); // Read bluetooth software serial
	
	tasker.runTasker();
}

