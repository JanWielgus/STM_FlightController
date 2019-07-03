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
void tempSerial();


enum stateType {disarmed, arming1, arming2, armed};
stateType state = disarmed;



void setup()
{
	// Communication serial
	Serial.begin(9600);
	delay(300);
	
	// Add functions to the tasker
	tasker.addFunction(updateMainCommunication, 40000L, 11);
	tasker.addFunction(readControlSticksValues, 20000L, 14); // 50Hz
	tasker.addFunction(updateLCD, 100000L, 10); // 10Hz
	tasker.addFunction(gestureRecognition, 100000L, 16); // 10Hz
	//tasker.addFunction(tempSerial, 40000L, 15); //25Hz
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
}


void gestureRecognition()
{
	static uint32_t counter = 0; // every 5 is one second
	
	int16_t thr, rot, tb, lr;
	thr = thrStick.getValue();
	rot = rotStick.getValue();
	tb = TB_Stick.getValue();
	lr = LR_Stick.getValue();
	
	
	// Arming / Disarming
	{
		static bool step1Passed = false;
		static bool step2Passed = false;
		static bool step3Passed = false;
		static uint32_t stepStartCounter = 0;
		if (state==disarmed || state==arming1 || state==arming2)
		{
			//////////////////
			// Arming
			//////////////////
			
			// Step 1
			if (step1Passed == false)
			{
				step2Passed = false;
				step3Passed = false;
				state = disarmed;
				
				// Idle position
				if (thr==0 && rot==0 && tb==0 && lr==0)
					step1Passed = true;
			}
			
			// Step 2
			if (step1Passed && step2Passed == false)
			{
				// Update step start counter to make time limit
				if (rot < 20)
					stepStartCounter = counter;
				
				// Failure
				if (thr > 5 || tb!=0 || lr!=0 || counter-stepStartCounter>6)
					step1Passed = false;
				
				// Step 3 detection
				if (rot > 450)
				{
					// Step 2 passed
					step1Passed = true;
					step2Passed = true;
					state = arming1;
					stepStartCounter = counter;
				}
			}
			
			// Step 3
			if (step1Passed && step2Passed && step3Passed == false)
			{
				// Failure
				if (rot < 450 || thr > 5 || tb !=0 || counter-stepStartCounter>10)
					step1Passed = false;
				
				// Step 4 detection
				if (lr < -450)
				{
					step1Passed = true;
					step2Passed = true;
					step3Passed = true;
					state = arming2;
					stepStartCounter = counter;
				}
			}
			
			// Step 4 - back to idle
			if (step3Passed)
			{
				// Failure
				if (counter-stepStartCounter > 7)
					step1Passed = false;
				
				// Armed detection
				if (thr==0 && rot==0 && tb==0 && lr==0)
				{
					step1Passed = false; // reset
					state = armed;
				}
			}
			
			// Arming end
			//////////////////
		}
		
		else if (state == armed)
		{
			//////////////////
			// Disarming
			//////////////////
			
			// Step 1
			if (step1Passed == false)
			{
				step2Passed = false;
				step3Passed = false;
				
				// Idle position
				if (thr==0 && rot==0 && tb==0 && lr==0)
					step1Passed = true;
			}
			
			// Step 2
			if (step1Passed && step2Passed == false)
			{
				// Update step start counter to make time limit
				if (rot > -20)
					stepStartCounter = counter;
				
				// Failure
				if (thr > 5 || tb!=0 || lr!=0 || counter-stepStartCounter>6)
					step1Passed = false;
				
				// Step 3 detection
				if (rot < -450)
				{
					// Step 2 passed
					step1Passed = true;
					step2Passed = true;
					stepStartCounter = counter;
				}
			}
			
			// Step 3
			if (step1Passed && step2Passed && step3Passed == false)
			{
				// Failure
				if (rot > -450 || thr > 5 || tb !=0 || counter-stepStartCounter>10)
					step1Passed = false;
				
				// Step 4 detection
				if (lr > 450)
				{
					step1Passed = true;
					step2Passed = true;
					step3Passed = true;
					stepStartCounter = counter;
				}
			}
			
			// Step 4 - back to idle
			if (step3Passed)
			{
				// Failure
				if (counter-stepStartCounter > 7)
					step1Passed = false;
				
				// Armed detection
				if (thr==0 && rot==0 && tb==0 && lr==0)
				{
					step1Passed = false; // reset
					state = disarmed;
				}
			}
			
			// Disarming end
			//////////////////
		}
	}
	
	
	
	counter++;
}


void tempSerial()
{
	//Serial.println(TB_Stick.getValue());
}
