// TaskerFunctions.h

#ifndef _TASKERFUNCTIONS_h
#define _TASKERFUNCTIONS_h

#include <FC_Task.h>


void addTaskerFunctionsToTasker();

namespace TaskerFunction
{
// General

	// built in diode is blinked once per second
	class Update1Hz : public FC_Task
	{
		void execute() override;
	};

	class Update75Hz : public FC_Task
	{
		void execute() override;
	};

	class UpdateMainInterval : public FC_Task
	{
		void execute() override;
	};

	class UpdateSending : public FC_Task
	{
		void execute() override;
	};


	
	// Tasks split into several methods
	void updateControlDiode();
	void readMPU6050();
	void readCompass();
	void processSlowerReadings();
	void newBaroReadingEvent(); // Function called by the baro instance




	// received packet events

	class SteeringReceivedUpdate : public FC_Task
	{
		void execute() override;
	};

	class BasicBackgroundReceivedUpdate : public FC_Task
	{
	protected:
		void execute() override;
	};

	class FullBackgroundReceivedUpdate : public BasicBackgroundReceivedUpdate
	{
		void execute() override;
	};

	class PID_paramsReceivedUpdate : public FC_Task
	{
		void execute() override;
	};
}


#endif

