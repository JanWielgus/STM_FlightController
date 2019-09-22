/*
    Created:	01/07/2019
    Author:     Jan Wielgus
*/

#include "Storage.h"
#include <FC_Tasker.h>
#include <FC_EVA_Filter.h>
#include <FC_Communication_Base.h>
#include <FC_CustomDataTypes.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "config.h"
#include "GestureRecognition.h"
#include "LCDhandler.h"
#include "BluetoothDevice.h"



// Tasker function prototypes
void updateSteeringSending();
void updateOtherSending();
void updateReceiving();
void readControlSticksValues();
void gestureRecognition();
void updateControlDiode();



void setup()
{
	// Communication serial
	Serial.begin(19200);
	delay(300);
	
	// bluetooth software serial
	btDevice::init();
	
	pinMode(LED_BUILTIN, OUTPUT);
	
	//switches
	pinMode(config::pin.leftSwitch, INPUT_PULLUP);
	pinMode(config::pin.rightSwitch, INPUT_PULLUP);
	
	
	// TEMPORARY !!!  for the radio module
	pinMode(config::pin.m0pin, OUTPUT);
	pinMode(config::pin.m1pin, OUTPUT);
	pinMode(config::pin.aux, INPUT);
	digitalWrite(config::pin.m0pin, HIGH);
	digitalWrite(config::pin.m1pin, LOW);
	
	// Add functions to the tasker
	tasker.addFunction(updateSteeringSending, 6000L, 160); // 166.6Hz - higher than drone receiving frequency to make communication uninterrupted !!!!! (tested duration)
	tasker.addFunction(updateOtherSending, 200000L, 170); // 5Hz (tested duration)
	tasker.addFunction(updateReceiving, 100000L, 730); // 10Hz (tested duration)
	tasker.addFunction(readControlSticksValues, 6000L, 730); // 166.6Hz (tested duration) ( !!!!  Actual frequency is 140Hz and same as steering sending - TO SOLVE  )
	tasker.addFunction(lcdH::updateLCD, 100000L, 2002); // 10Hz (tested duration ? not sure if is real)
	tasker.addFunction(gestureRecognition, 100001L, 20); // 10Hz (without 1 at the end is 7/8Hz) (tested duration)
	tasker.addFunction(updateControlDiode, 1000000L, 5); // blink built in diode every second
	tasker.addFunction(btDevice::handleReceivedData, 500000L, 1); // 2Hz do something with data received from the bluetooth device
	//tasker.scheduleTasks();
	
	
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

	
	lcdH::initLCD();
	
	// Necessary for LCD to keep up
	Wire.setClock(400000L);
}


void loop()
{
	// Read bluetooth software serial
	btDevice::read();
	
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
	
	temp_counter = counter;
	counter = 0;
}


void updateSteeringSending()
{
	com.toSend.steer.throttle = thrStick.getValue();
	com.toSend.steer.rotate = rotStick.getValue();
	com.toSend.steer.TB = TB_Stick.getValue();
	com.toSend.steer.LR = LR_Stick.getValue();
	
	com.packAndSendData(com.sendPacketTypes.TYPE4_ID, com.sendPacketTypes.TYPE4_SIZE);
}


void updateOtherSending()
{
	com.toSend.arming = state==armed ? 1 : 0;

	// Set the flight modes
	if (digitalRead(config::pin.rightSwitch) == LOW)
		com.toSend.flightMode = 0; // satbilize
	else
		com.toSend.flightMode = 1; // altHold

	// OTHER DATA !!!
	// send packed data
	//com.packAndSendData(com.sendPacketTypes.TYPE2_ID, com.sendPacketTypes.TYPE2_SIZE);
	com.packAndSendData(com.sendPacketTypes.TYPE1_ID, com.sendPacketTypes.TYPE1_SIZE);
	
	
	// check if need to send PID tuning values
	if (btNeedToUpdatePIDFlag)
	{
		btNeedToUpdatePIDFlag = false;
		
		com.toSend.PIDcontrollerID = btControllerID;
		com.toSend.PIDvalues.P = btPID_P;
		com.toSend.PIDvalues.I = btPID_I;
		com.toSend.PIDvalues.I_max = btPID_Imax;
		com.toSend.PIDvalues.D = btPID_D;
		
		com.packAndSendData(com.sendPacketTypes.TYPE3_ID, com.sendPacketTypes.TYPE3_SIZE);
	}
}


void updateReceiving()
{
	com.receiveAndUnpackData();
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

