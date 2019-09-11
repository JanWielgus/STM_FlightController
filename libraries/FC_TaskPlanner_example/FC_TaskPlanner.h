// FC_TaskPlanner.h
/*
    Name:       FC_TaskPlanner_example.ino
    Created:	10/09/2019 21:32:39
    Author:     Jan Wielgus
*/

#ifndef _FC_TASKPLANNER_h
#define _FC_TASKPLANNER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


class FC_TaskPlanner
{
 private:
	typedef void (*functionPointer)();
	
 public:
	FC_TaskPlanner(uint8_t plannedTasksMaxArrSize = 5); // max amt of planned tasks at one time
	~FC_TaskPlanner();
	bool scheduleTask(functionPointer fPtr, uint16_t call_in); // callIn - in how many milliseconds call a scheduled task; return true if added successfully
	void runPlanner(); // execute this function in the main loop()
	
 private:
	bool removeTaskFromArray(uint8_t taskPos); // used after function execution, remove empty space from the array (0 is the first task)
	
	
 private:
	const uint8_t MaxPlannedTasks;
	
	struct plannedTaskType
	{
		functionPointer functionPtr;
		uint32_t timeToExecute;
	};
	plannedTaskType* plannedTasksArr; // array where stored are functions until they are called
	uint8_t tasksInArray = 0; // amt of tasks in the array above
};


#endif

