// Author: Jan Wielgus
// Date: 23.11.2018
//

#include "FC_Tasker.h"


volatile bool FC_Tasker::baseLoopFlag = false;
volatile uint16_t FC_Tasker::baseLoopCounter = 0;


void baseLoopTimerHandler()
{
	FC_Tasker::baseLoopFlag = true;
	FC_Tasker::baseLoopCounter++; // increment the counter
}


FC_Tasker::FC_Tasker(uint16_t base_interval) : BASE_INTERVAL(base_interval)
{
	Timer2.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
	Timer2.setPeriod(BASE_INTERVAL); // in microseconds
	Timer2.setCompare(TIMER_CH1, 1);      // overflow might be small ???
	Timer2.attachInterrupt(TIMER_CH1, baseLoopTimerHandler);
}


FC_Tasker::~FC_Tasker()
{
	if (numberOfTasks > 0)
	{
		delete [] taskList;
		delete mainTask;
	}
}


// Add a new task at the end of dynamically created array
void FC_Tasker::addFunction( void (*funcPointer)(), long interv, uint16_t maxDur )
{
	numberOfTasks++;
	
	// create new tasks container
	Task * newTaskList = new Task[numberOfTasks];

	// copy old tasks to the new container and delete old one
	copyTaskList(taskList, newTaskList, numberOfTasks-1);
	delete [] taskList;
	
	// add new task at the end
	newTaskList[numberOfTasks-1].functionPointer = funcPointer;
	newTaskList[numberOfTasks-1].interval = interv;
	newTaskList[numberOfTasks-1].maxDuration = maxDur;
	taskList = newTaskList;
}


// Executed once. Dynamically add the main task
void FC_Tasker::addMainFunction( void (*mainFuncPointer)(), long interv, uint16_t maxDur )
{
	mainTask = new Task;
	mainTask->functionPointer = mainFuncPointer;
	mainTask->interval = interv;
	mainTask->maxDuration = maxDur;
}


void FC_Tasker::runTasker()
{
	//....
	// if (baseLoopCounter == ...) then execute main function, at the end check if counter rised and if so, it means that system is unstable
}


// Copy one Tasks array to another
void FC_Tasker::copyTaskList(Task *from, Task *to, uint8_t amount)
{
	for (int i=0; i < amount; i++)
	{
		to->functionPointer = from->functionPointer;
		to->interval = from->interval;
		to->maxDuration = from->maxDuration;
	}
}


