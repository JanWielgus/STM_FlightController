// TaskerFunctions.h

#ifndef _TASKERFUNCTIONS_h
#define _TASKERFUNCTIONS_h

#include <FC_Task.h>


void addTaskerFunctionsToTasker();

namespace TaskerFunction
{
// General

	// built in diode is blinked once per second
	class UpdateControlDiode : public FC_Task
	{
		bool ledState = LOW;
		void execute() override;
	};

	// Check if there is a need to calibrate one of the module and perform it if needed
	class CheckCalibrations : public FC_Task
	{
		void execute() override;
	};
	



// Steering

	class ReadXY_angles : public FC_Task
	{
		void execute() override;
	};

	class ReadCompass : public FC_Task
	{
		void execute() override;
	};

	void newBaroReadingEvent(); // Function called by the baro instance

	class ProcessSlowerReadings : public FC_Task
	{
		void execute() override;
	};




// Communication

	class UpdateSending : public FC_Task
	{
		void execute() override;
	};

	class UpdateReceiving : public FC_Task
	{
		void execute() override;
	};
}


#endif

