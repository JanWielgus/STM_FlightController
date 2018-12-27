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


FC_Tasker::FC_Tasker( void (*mainFuncPointer)(), long interv, uint16_t maxDur ) : BASE_INTERVAL(interv)
{
	/*
	Timer2.setMode(TIMER_CH1, TIMER_OUTPUTCOMPARE);
	Timer2.setPeriod(BASE_INTERVAL); // in microseconds
	Timer2.setCompare(TIMER_CH1, 1);      // overflow might be small ???
	Timer2.attachInterrupt(TIMER_CH1, baseLoopTimerHandler);
	*/
	
	timer2 = new HardwareTimer(2);
	timer2->pause();
	timer2->setPeriod(BASE_INTERVAL);
	timer2->setChannel1Mode(TIMER_OUTPUT_COMPARE);
	timer2->setCompare(TIMER_CH1, 1);
	timer2->attachCompare1Interrupt(baseLoopTimerHandler);
	timer2->refresh();
	timer2->resume();
}


FC_Tasker::~FC_Tasker()
{
	if (amtOfTasks > 0)
		delete [] taskList;
}


// Add a new task at the end of dynamically created array
void FC_Tasker::addFunction( void (*funcPointer)(), long interv, uint16_t maxDur )
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
	
	// create array of bools to know if for certain task shift have been already set
	bool* isShiftSet = new bool[amtOfTasks]; // true - shift is set for that function and other with the same interval
	for (int i=0; i<amtOfTasks; i++) isShiftSet = false;
	
	for (int i=0; i<amtOfTasks; i++)
	{
		// if shift for that interval have been already set -> skip
		if (isShiftSet[i] == true)
			continue;
			
		int smallerInt; // created there to use in the next for loop
		
		// array of pointers to the tasks with the same interval
		Task** sameIntTasks = new Task*[amtOfTasks];
		int amtOfSameIntTasks = 0;
		
		// look for and add to the sameIntTasks array tasks which have the same interval
		for (int j=i+1; j<amtOfTasks; j++)
		{
			// if shift for that interval have been already set -> skip
			if (isShiftSet[j] == true)
				continue;
			
			// Bigger and smaller interval
			smallerInt = min(taskList[i].interval, taskList[j].interval);
			int biggerInt = max(taskList[i].interval, taskList[j].interval);
			
			if (biggerInt % smallerInt == 0) // If is divisible
			{
				/* checking this is meaningless because 'i' is always different than 'j', and in one i iteration there can't be the same two functions. isShiftSet array is checked at the beginning of the both 'i' and 'j'
				// if not contain then add
				if (checkIfContain(sameIntTasks, amtOfSameIntTasks, &(taskList[i])) == false)
					sameIntTasks[amtOfSameIntTasks++] = &taskList[i];
				if (checkIfContain(sameIntTasks, amtOfSameIntTasks, &(taskList[j])) == false)
					sameIntTasks[amtOfSameIntTasks++] = &taskList[j];
				*/
				if (!isShiftSet[i]) // 'i' is the same for one 'j' loop so it must be checked to to avoid duplicates
					sameIntTasks[amtOfSameIntTasks++] = &taskList[i];
				sameIntTasks[amtOfSameIntTasks++] = &taskList[j]; // 'j' is always new
					
				isShiftSet[i] = true;
				isShiftSet[j] = true;
			}
		}
		
		
		// if there were zero tasks with the same interval -> skip
		if(amtOfSameIntTasks < 2)
		{
			isShiftSet[i] = true;
			continue;
		}
		
		//calculate the shift for tasks with the same interval
		float baseShift = float(smallerInt)/amtOfSameIntTasks; // eg.: =5 then shift1=5, shift2=10, shift3=15...
		for (int q=1; q<amtOfSameIntTasks; q++)
		{
			sameIntTasks[q]->shift = uint16_t(q * baseShift); // calculate the next shifts
		}
		
		delete [] sameIntTasks;
	}
	
	delete [] isShiftSet;
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
	/*
		Reszte zadan ma byc wywolywane na podstawie czasu jaki uplynal od ostatniego wykonania, uzywajac micros() i dodajac shift
	*/
	
	for (uint8_t i=0; i<amtOfTasks; i++)
	{
		static uint32_t curTime = 0; // time now
		
		curTime = micros();
		
		//if time has elapsed -> execute the task
		if (curTime > (taskList[i].lastExecuteTime + taskList[i].interval + taskList[i].shift))
		{
			taskList[i].lastExecuteTime = curTime;
			(*taskList[i].functionPointer)();
		}
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

/*
bool FC_Tasker::checkIfContain(Task** source, int amt, Task* toCheck)
{
	for (int i=0; i<amt; i++)
	{
		if (source[i]->functionPointer == toCheck->functionPointer)
			return true;
	}
	return false;
}
*/

