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
	
	FlightMode flightMode = 0;
	
	
	void runVirtualPilot(); // !!! run this in the same frequency as stabilize() (can be inside)
	void detectFlightModeChange();
	
	
	
	//////////////////////////////////////////////////////////////////////////
	
	
	void runVirtualPilot()
	{
		detectFlightModeChange();
		
		
		if (flightMode <= POS_HOLD)
		{
			// copy raw stick variables for proper flight modes
			switch (flightMode)
			{
			case STABILIZE:
				vSticks.throttle = com.received.steer.throttle;
				// do not break
			case ALT_HOLD:
				vSticks.TB = extrapolatedTBstick;
				vSticks.LR = extrapolatedLRstick;
				// do not break
			case POS_HOLD:
				vSticks.rotate = com.received.steer.rotate;
			}
			

			// set up the flight modes
			switch (flightMode)
			{
			/* by now, for this flight modes there is no need to do anything
			case POS_HOLD:
				// position hold flight mode code

				// CALCULATE THERE vStick.TB and vStick.LR
				
				// do not break to keep altitude and stabilize
				
				
			case ALT_HOLD:
				// altHold pid controller is updated in the ino file in 110Hz
				// flag to update is changed when setting the new flight mode

				// CALCULATE THERE pressure to hold
				
				// do not break to keep stabilize
			*/
				
			case STABILIZE:
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
		static FlightMode lastFlightMode = STABILIZE; // to enable setting fMode 0 for the first times
		
		if (com.received.flightMode == lastFlightMode) // if flight mode has not changed then do nothing
			return;
		
		if (com.received.flightMode <= POS_HOLD) // other flight modes will be handled in a different way
		{
			flightMode = com.received.flightMode;
			
			switch (flightMode)
			{
				case POS_HOLD:
					// RESET THE POS HOLD PID CONTROLLER
				
					if (lastFlightMode == ALT_HOLD) // break if changing from altHold to posHold (do not need to reset altHold controller)
						break;
				
				
				case ALT_HOLD:
					altHoldPID.resetController();
					needToUpdateAltHoldPID_flag = true;
				
					// do other necessary actions
					break;
				
				
				case STABILIZE:
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