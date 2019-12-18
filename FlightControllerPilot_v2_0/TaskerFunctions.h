#ifndef TASKERFUNCTIONS_H_
#define TASKERFUNCTIONS_H_

void addTaskerFunctionsToTasker();

namespace TaskerFunction
{
	// box_... is when only execute a method inside

	// Communication
	void updateSteeringSending();
	void updateOtherSending();
	void box_updateReceiving();

	// Sticks
	void readControlSticksValues();
	void box_gestureRecognition();

	// Display
	void box_updateDisplayFast();
	void box_updateDisplaySlow();

	// Other
	void updateControlDiode();
	void box_androidHandleReceivedData();
	
}


#endif /* TASKERFUNCTIONS_H_ */