#include "TaskerFunctions.h"
#include "Storage.h"
#include "config.h"


void addTaskerFunctionsToTasker()
{
	using namespace TaskerFunction;

	// Communication
	tasker.addFunction(updateSteeringSending, 6000L, 160); // 166.6Hz - higher than drone receiving frequency to make communication uninterrupted !!!!! (tested duration)
	tasker.addFunction(updateOtherSending, 200000L, 170); // 5Hz (tested duration)
	tasker.addFunction(box_updateReceiving, 100000L, 730); // 10Hz (tested duration)

	// Sticks
	tasker.addFunction(readControlSticksValues, 6000L, 730); // 166.6Hz (tested duration) ( !!!!  Actual frequency is 140Hz and same as steering sending - TO SOLVE  )
	tasker.addFunction(box_gestureRecognition, 100001L, 20); // 10Hz (without 1 at the end is 7-8Hz) (tested duration)

	// Display
	tasker.addFunction(box_updateDisplayFast, 100000L, 2002); // 10Hz (tested duration ? not sure if is real)
	tasker.addFunction(box_updateDisplaySlow, 333333L, 0); // 3Hz

	// Others
	tasker.addFunction(updateControlDiode, 1000000L, 5); // blink built in diode every second
	tasker.addFunction(box_androidHandleReceivedData, 500000L, 1); // 2Hz do something with data received from the bluetooth device


	//tasker.scheduleTasks(); // not working
}



namespace TaskerFunction
{
	void updateControlDiode()
	{
		static bool ledState = LOW;
		digitalWrite(LED_BUILTIN, ledState);
		ledState = !ledState;
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
		// Update sent values before sending

		com.toSend.arming = armState == armed ? 1 : 0;

		// Set the flight modes  ( FLIGHT MODES SYSTEM WILL CHANGE SO DO THIS )
		if (digitalRead(config::pin.rightSwitch) == LOW)
			com.toSend.flightMode = 0; // stabilize
		else
			com.toSend.flightMode = 1; // altHold


		// OTHER DATA !!!


		// send packed data
		//com.packAndSendData(com.sendPacketTypes.TYPE2_ID, com.sendPacketTypes.TYPE2_SIZE);
		com.packAndSendData(com.sendPacketTypes.TYPE1_ID, com.sendPacketTypes.TYPE1_SIZE);


		// check if need to send PID tuning values
		if (flags.btNeedToUpdatePID)
		{
			flags.btNeedToUpdatePID = false;

			com.toSend.PIDcontrollerID = androidData.controllerID;
			com.toSend.PIDvalues.P = androidData.PID_P;
			com.toSend.PIDvalues.I = androidData.PID_I;
			com.toSend.PIDvalues.I_max = androidData.PID_Imax;
			com.toSend.PIDvalues.D = androidData.PID_D;

			com.packAndSendData(com.sendPacketTypes.TYPE3_ID, com.sendPacketTypes.TYPE3_SIZE);
		}
	}



	void box_updateReceiving()
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



	void box_gestureRecognition()
	{
		gestureRecognizer.recognizeArmingAndDisarmingGesture();
	}



	void box_updateDisplayFast()
	{
		display.updateFastParts();
	}



	void box_updateDisplaySlow()
	{
		display.updateSlowParts();
	}


	void box_androidHandleReceivedData()
	{
		androidCom.handleReceivedData();
	}
}

