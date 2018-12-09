// Author: Jan Wielgus
// Date: 23.11.2018
//

#include "FC_Tasker.h"


volatile bool FC_Tasker::baseLoopFlag = false;

// Now do not used
//volatile uint32_t FC_Tasker::baseLoopCounter = 0;
//volatile uint32_t FC_Tasker::mainLoopCounter = 0;


void baseLoopTimerHandler()
{
	FC_Tasker::baseLoopFlag = true;
	//FC_Tasker::baseLoopCounter++; // increment the counter
	//if ((FC_Tasker::baseLoopCounter % 4) == 0) FC_Tasker::mainLoopCounter++; // ONLY IF MAIN LOOP IS 250Hz AND BASE_INTERVAL IS 500 !!!!!!!!!!! - this part have to be implemented in a better way 
}


FC_Tasker::FC_Tasker( void (*mainFuncPointer)(), long interv, uint16_t maxDur ) : BASE_INTERVAL(interv);
{
	Timer2.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
	Timer2.setPeriod(BASE_INTERVAL); // in microseconds
	Timer2.setCompare(TIMER_CH1, 1);      // overflow might be small ???
	Timer2.attachInterrupt(TIMER_CH1, baseLoopTimerHandler);
}


FC_Tasker::~FC_Tasker()
{
	if (numberOfTasks > 0)
		delete [] taskList;
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
	newTaskList[numberOfTasks-1].lastExecuteTime = 0;
	taskList = newTaskList;
}

/* - now in the constructor
// Executed once. Add the main task
void FC_Tasker::addMainFunction( void (*mainFuncPointer)(), long interv, uint16_t maxDur )
{
	mainTask.functionPointer = mainFuncPointer;
	mainTask.interval = interv;
	mainTask.maxDuration = maxDur;
}
*/

void FC_Tasker::runTasker()
{
	if (baseLoopFlag)
	{
		mainTask.lastExecuteTime = micros();
		
		// execute the main task
		(*mainTask.functionPointer)();
		
		mainTaskDuration = micros() - mainTask.lastExecuteTime;
		
		// If this flag will be true -> that means system is overloaded
		if (baseLoopFlag)
		{
			flag.systemOverloaded = true;
			flag.systemUnstable = true;
		}
		else flag.systemOverloaded = true;
	}
	
	
	// Running other tasks
	//.......
	//......
	/*
		Reszte zadan ma byc wywolywane na podstawie czasu jaki uplynal od ostatniego wykonania, uzywajac micros()
	*/
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


