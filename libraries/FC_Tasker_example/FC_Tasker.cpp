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
	if (amtOfTasks > 0)
		delete [] taskList;
}


// Add a new task at the end of dynamically created array
void FC_Tasker::addFunction( void (*funcPointer)(), long interv, uint16_t maxDur )
{
	amtOfTasks++;
	
	// create new tasks container
	Task * newTaskList = new Task[amtOfTasks];

	// copy old tasks to the new container and delete old one
	copyTaskList(taskList, newTaskList, amtOfTasks-1);
	delete [] taskList;
	
	// add new task at the end
	newTaskList[amtOfTasks-1].functionPointer = funcPointer;
	newTaskList[amtOfTasks-1].interval = interv;
	newTaskList[amtOfTasks-1].maxDuration = maxDur;
	newTaskList[amtOfTasks-1].lastExecuteTime = 0;
	newTaskList[amtOfTasks-1].shift = 0;
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

void FC_Tasker::scheduleTasks()
{
	/*
		Trzeba sprawdzic kazde z kazdym i zobaczyc czy interval nie jest wielokrotnoscia (czyli czy % == 0)
	*/
	for (int i=0; i<amtOfTasks; i++)
	{
		for (int j=i+1; j<amtOfTasks; j++)
		{
			// Bigger and smaller interval
			int smallerInt = min(taskList[i].interval, taskList[j].interval);
			int biggerInt = max(taskList[i].interval, taskList[j].interval);
			
			if (biggerInt % smallerInt == 0) // If is divisible
			{
				// ...................
				// there calculate the shift base and if following ifs are true -> set them shifts
				// be aware of that shifts might be set before for the given interval so u have to do sth
				if (taskList[i].shift == 0) // if it hasn't had any shift yet
				{
					
				}
				
				if (taskList[j].shift == 0) // if it hasn't had any shift yet
				{
					
				}
				
			}
		}
	}
}


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
	
	for (uint8_t i=0; i<amtOfTasks; i++)
	{
		if ()
	}
}


// Copy one Tasks array to another
void FC_Tasker::copyTaskList(Task *from, Task *to, uint8_t amount)
{
	for (int i=0; i < amount; i++)
	{
		to->functionPointer = from->functionPointer;
		to->interval = from->interval;
		to->maxDuration = from->maxDuration;
		to->lastExecuteTime = from->lastExecuteTime;
		to->shift = from->shift;
	}
}


