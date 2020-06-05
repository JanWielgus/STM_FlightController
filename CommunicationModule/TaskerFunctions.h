// TaskerFunctions.h

#ifndef _TASKERFUNCTIONS_h
#define _TASKERFUNCTIONS_h

#include "arduino.h"
#include <FC_Task.h>

void addTaskerFunctionsToTasker();

namespace TaskerFunction
{
	class OneHertzUpdate : public FC_Task
	{
		void execute() override;
	};


	class UpdateCommunicationRelaying : public FC_Task
	{
		void execute() override;
	};
}




#endif

