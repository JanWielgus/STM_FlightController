// TaskerFunctions.h

#ifndef _TASKERFUNCTIONS_h
#define _TASKERFUNCTIONS_h

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


	/*
	// !!!!!
	// THIS PART HAVE TO BE REMOVED (ONLY VIRTUAL PILOT USE FLIGHT MODES)
	class Stabilize : public FC_Task
	{
		void execute() override;
	};*/


	//void updatePressureAndAltHold(); // update altHold PID if needed and do other baro stuff




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

