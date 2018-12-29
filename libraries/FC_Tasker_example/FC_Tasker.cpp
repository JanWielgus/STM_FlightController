// Author: Jan Wielgus
// Date: 23.11.2018
//

#include "FC_Tasker.h"

//////////////////////////////////////////////////////////////////////////
//                FC_SimpleTasker        \/  \/  \/
//////////////////////////////////////////////////////////////////////////


FC_SimpleTasker::FC_SimpleTasker()
{
	// nothing
}


FC_SimpleTasker::~FC_SimpleTasker()
{
	if (amtOfTasks > 0)
	delete [] taskList;
}


// Add a new task at the end of dynamically created array
void FC_SimpleTasker::addFunction( void (*funcPointer)(), long interv, uint16_t maxDur )
{
	// check if there is too much tasks
	if (amtOfTasks+1 > MAX_AMT_OF_TASKS)
	return;
	
	amtOfTasks++;
	
	// create new tasks container
	Task * newTaskList = new Task[amtOfTasks];

	// copy old tasks to the new container and delete old one
	copyTaskList(taskList, newTaskList, amtOfTasks);
	delete [] taskList;
	
	// add new task at the end
	newTaskList[amtOfTasks-1].functionPointer = funcPointer;
	newTaskList[amtOfTasks-1].interval = interv;
	newTaskList[amtOfTasks-1].maxDuration = maxDur;
	newTaskList[amtOfTasks-1].lastExecuteTime = 0;
	newTaskList[amtOfTasks-1].timeShift = 0;
	taskList = newTaskList;
}


void FC_SimpleTasker::scheduleTasks()
{	
	for (int i=0; i<amtOfTasks; i++)
	{
		for (int j=i+1; j<amtOfTasks; j++)
		{
			int bigger = max(taskList[i].interval+taskList[i].timeShift, taskList[j].interval+taskList[j].timeShift);
			int smaller = min(taskList[i].interval+taskList[i].timeShift, taskList[j].interval+taskList[j].timeShift);
			
			if (bigger % smaller == 0) // is divisible
				taskList[j].timeShift += int(taskList[i].interval/3) + taskList[i].maxDuration + TIME_SHIFT_BASE; // calculate new shift for second task, it's not ideal, but works
		}
	}
}


void FC_SimpleTasker::runTasker()
{
	for (uint8_t i=0; i<amtOfTasks; i++)
	{
		static uint32_t curTime = 0; // time now
		
		curTime = micros();
		
		//if time has elapsed -> execute the task
		if (curTime > (taskList[i].lastExecuteTime + taskList[i].interval + taskList[i].timeShift))
		{
			taskList[i].lastExecuteTime = curTime;
			(*taskList[i].functionPointer)();
		}
	}
}


// Copy one Tasks array to another
void FC_SimpleTasker::copyTaskList(Task *from, Task *to, uint8_t amount)
{
	for (int i=0; i < amount; i++)
	{
		to->functionPointer = from->functionPointer;
		to->interval = from->interval;
		to->maxDuration = from->maxDuration;
		to->lastExecuteTime = from->lastExecuteTime;
		to->timeShift = from->timeShift;
	}
}

/*
bool FC_SimpleTasker::checkIfContain(Task** source, int amt, Task* toCheck)
{
	for (int i=0; i<amt; i++)
	{
		if (source[i]->functionPointer == toCheck->functionPointer)
			return true;
	}
	return false;
}
*/





//////////////////////////////////////////////////////////////////////////
//                FC_Tasker        \/  \/  \/
//////////////////////////////////////////////////////////////////////////


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


FC_Tasker::FC_Tasker( void (*mainFuncPointer)(), long interv, uint16_t maxDur ) : BASE_INTERVAL(interv), FC_SimpleTasker()
{
	/*
	Timer2.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
	Timer2.setPeriod(BASE_INTERVAL); // in microseconds
	Timer2.setCompare(TIMER_CH1, 1);      // overflow might be small ???
	Timer2.attachInterrupt(TIMER_CH1, baseLoopTimerHandler);
	*/
	
	mainTask.functionPointer = mainFuncPointer;
	mainTask.interval = interv;
	mainTask.maxDuration = maxDur;
	
	timer2 = new HardwareTimer(2); // Hardware Timer 2 (STM32 BluePill)
	timer2->pause();
	timer2->setPeriod(BASE_INTERVAL);
	timer2->setChannel1Mode(TIMER_OUTPUT_COMPARE);
	timer2->setCompare(TIMER_CH1, 1);
	timer2->attachCompare1Interrupt(baseLoopTimerHandler);
	timer2->refresh();
	timer2->resume();
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
		
		baseLoopFlag = false;
	}
	
	
	// Running other tasks
	/*
		Reszte zadan ma byc wywolywane na podstawie czasu jaki uplynal od ostatniego wykonania, uzywajac micros() i dodajac shift
	*/
	
	FC_SimpleTasker::runTasker();
}

