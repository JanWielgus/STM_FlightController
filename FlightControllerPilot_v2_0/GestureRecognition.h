/*
 * GestureRecognition.h
 *
 * Created: 20/07/2019 17:35:02
 *  Author: janwi
 */ 


#ifndef GESTURERECOGNITION_H_
#define GESTURERECOGNITION_H_


enum stateType {disarmed, arming1, arming2, armed};
stateType state = disarmed;


namespace gr // gesture recognition
{
	void recognizeArmingAndDisarmingGesture(int16_t thr, int16_t rot, int16_t tb, int16_t lr) // 10Hz
	{
		static uint32_t counter = 0; // every 5 is one second
		
		
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
	
	
	
	
	
	
}




#endif /* GESTURERECOGNITION_H_ */