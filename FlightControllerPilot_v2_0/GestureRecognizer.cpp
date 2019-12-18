#include "GestureRecognizer.h"
#include "Storage.h"
#include "config.h"


GestureRecognizer::GestureRecognizer()
{
	// by now do nothing
}


// 10Hz
void GestureRecognizer::recognizeArmingAndDisarmingGesture()
{
	static uint32_t counter = 0; // every 5 is one second

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
		if (armState == disarmed || armState == arming1 || armState == arming2)
		{
			//////////////////
			// Arming
			//////////////////

			// Step 1
			if (step1Passed == false)
			{
				step2Passed = false;
				step3Passed = false;
				armState = disarmed;

				// Idle position
				if (thr == 0 && rot == 0 && tb == 0 && lr == 0 && (digitalRead(config::pin.rightSwitch) == LOW))
					step1Passed = true;
			}

			// Step 2
			if (step1Passed && step2Passed == false)
			{
				// Update step start counter to make time limit
				if (rot < 20)
					stepStartCounter = counter;

				// Failure
				if (thr > 5 || tb != 0 || lr != 0 || counter - stepStartCounter > 6)
					step1Passed = false;

				// Step 3 detection
				if (rot > 450)
				{
					// Step 2 passed
					step1Passed = true;
					step2Passed = true;
					armState = arming1;
					stepStartCounter = counter;
				}
			}

			// Step 3
			if (step1Passed && step2Passed && step3Passed == false)
			{
				// Failure
				if (rot < 450 || thr > 5 || tb != 0 || counter - stepStartCounter > 10)
					step1Passed = false;

				// Step 4 detection
				if (lr < -450)
				{
					step1Passed = true;
					step2Passed = true;
					step3Passed = true;
					armState = arming2;
					stepStartCounter = counter;
				}
			}

			// Step 4 - back to idle
			if (step3Passed)
			{
				// Failure
				if (counter - stepStartCounter > 7)
					step1Passed = false;

				// Armed detection
				if (thr == 0 && rot == 0 && tb == 0 && lr == 0)
				{
					step1Passed = false; // reset
					armState = armed;
				}
			}

			// Arming end
			//////////////////
		}

		else if (armState == armed)
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
				if (thr == 0 && rot == 0 && tb == 0 && lr == 0)
					step1Passed = true;
			}

			// Step 2
			if (step1Passed && step2Passed == false)
			{
				// Update step start counter to make time limit
				if (rot > -20)
					stepStartCounter = counter;

				// Failure
				if (thr > 5 || tb != 0 || lr != 0 || counter - stepStartCounter > 6)
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
				if (rot > -450 || thr > 5 || tb != 0 || counter - stepStartCounter > 10)
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
				if (counter - stepStartCounter > 7)
					step1Passed = false;

				// Armed detection
				if (thr == 0 && rot == 0 && tb == 0 && lr == 0)
				{
					step1Passed = false; // reset
					armState = disarmed;
				}
			}

			// Disarming end
			//////////////////
		}
	}



	counter++;
}

