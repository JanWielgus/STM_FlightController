/*
 * FlightModes.h
 *
 * Created: 17/09/2019 10:04:56
 *  Author: Jan Wielgus
 *
 *
 * Flight modes works as virtual pilot, which takes none, part or full control of the vehicle
 * Drone have to use virtual sticks (vSticks)
 */ 


#ifndef FLIGHTMODES_H_
#define FLIGHTMODES_H_

#include "Storage.h"
#include "config.h"


namespace fModes
{
	struct
	{
		uint16Byte throttle;
		int16Byte rotate;
		int16Byte TB;
		int16Byte LR;
	} vSticks; // virtual sticks
	
	uint8_t flightMode = 0;
	
	
	void runVirtualPilot(); // !!! run this in the same frequency as stabilize() (can be inside)
	void detectFlightModeChange();
	
	
	
	//////////////////////////////////////////////////////////////////////////
	
	
	void runVirtualPilot()
	{
		detectFlightModeChange();
		
		
		if (flightMode <= 2)
		{
			// copy raw stick variables for proper flight modes
			switch (flightMode)
			{
			case 0:
				vSticks.throttle = com.received.steer.throttle;
				// do not break
			case 1:
				vSticks.TB = extrapolatedTBstick;
				vSticks.LR = extrapolatedLRstick;
				// do not break
			case 2:
				vSticks.rotate = com.received.steer.rotate;
			}
			

			// set up the flight modes
			switch (flightMode)
			{
			/* by now, for this flight modes there is no need to do anything
			case 2: // posHold
				// position hold flight mode code

				// CALCULATE THERE vStick.TB and vStick.LR
				
				// do not break to keep altitude and stabilize
				
				
			case 1: // altHold
				// altHold pid controller is updated in the ino file in 110Hz
				// flag to update is changed when setting the new flight mode

				// CALCULATE THERE vStick.throttle
				
				// do not break to keep stabilize
			*/
				
			case 0: // stabilize
				// leveling PID
				pidXval = levelXpid.updateController(angle.x + (vSticks.TB/10)) + 0.5;
				pidYval = levelYpid.updateController(angle.y - (vSticks.LR/10)) + 0.5;
					
				// yaw PID
				float headingError = headingToHold - heading;
				if (headingError >= 180)
					headingError -= 360;
				else if (headingError <= -180)
					headingError += 360;
				pidYawVal = yawPID.updateController(headingError);
					
				
				// set controllers to zero if throttle is low
				if (vSticks.throttle < config::ZeroActionThrottle)
				{
					pidXval = 0;
					pidYval = 0;
					pidYawVal = 0;
					
					// reset the headingToHold
					headingToHold = heading;
				}
					
				break;
				
			}
			
		}
	}
	
	
	void detectFlightModeChange()
	{
		static uint8_t lastFlightMode = 0; // to enable setting fMode 0 for the first times
		
		if (com.received.flightMode == lastFlightMode) // if flight mode has not changed then do nothing
			return;
		
		if (com.received.flightMode <= 2) // other flight modes will be handled in a different way
		{
			flightMode = com.received.flightMode;
			
			switch (flightMode)
			{
				case 2: // posHold
					// RESET THE POS HOLD PID CONTROLLER
				
					if (lastFlightMode == 1) // break if changing from altHold to posHold (do not need to reset altHold controller)
						break;
				
				
				case 1: // altHold
					altHoldPID.resetController();
					needToUpdateAltHoldPID_flag = true;
				
					// do other necessary actions
					break;
				
				
				case 0:
					needToUpdateAltHoldPID_flag = false;
			}
			
		}
		else
		{
			// Other flight modes (special flight modes)
			// could be different variable with special flight modes
			// this if else will decide how to change both flight mode and special flight mode variables
			// this is because special flight modes use basic ones
			
			// ! flightMode is set in previous if part !
		}
		
		
		lastFlightMode = flightMode;
	}
	
	
}



#endif /* FLIGHTMODES_H_ */