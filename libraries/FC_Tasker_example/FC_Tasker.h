// FC_Tasker.h
//
// Author: Jan Wielgus
// Date: 23.11.2018\
//
/*
	It use STM Timer2 so make sure that all pins (mainly Serial1) will work correctly
*/

#ifndef _FC_TASKER_h
#define _FC_TASKER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


class FC_SimpleTasker
{
 public:
	FC_SimpleTasker();
	~FC_SimpleTasker();
	void addFunction( void (*funcPointer)(), long interv, uint16_t maxDur );
	void scheduleTasks(); // plan the tasks shifts
	virtual void runTasker(); // should be the only function in loop. Execute tasks in intelligent way, not everything at one time
	
	
 protected:
	struct Task
	{
		void (*functionPointer)(); // pointer to the function
		long interval; // in milliseconds
		uint16_t maxDuration; // in milliseconds - input by user
		uint32_t lastExecuteTime; // in microseconds, time when this function was lately called
		uint16_t timeShift; // in microseconds, to avoid running in the same moment several tasks with the same interval
	};


 private:
	const uint8_t MAX_AMT_OF_TASKS = 250;
	const uint8_t TIME_SHIFT_BASE = 239; // used to calculate time shift
	
	Task * taskList = nullptr; // dynamically created, list of tasks, except main task (it is in other pointer)
	uint8_t amtOfTasks = 0;
	
	void copyTaskList(Task *from, Task *to, uint8_t amount);
	//bool checkIfContain(Task** source, int amt, Task* toCheck); // checking if in the array of pointers 'source' is 'toCheck'
};



//////////////////////////////////////////////////////////////////////////



//class FC_Tasker : public FC_SimpleTasker
//{
// public:
//	FC_Tasker( void (*mainFuncPointer)(), long interv, uint16_t maxDur );
//	//void addMainFunction( void (*mainFuncPointer)(), long interv, uint16_t maxDur ); // - now in the constructor
//	void runTasker(); // (virtual) should be the only function in loop. Execute tasks like in SimpleTasker and one main task
//
// private:
//	HardwareTimer* timer2;
//	
//	const uint32_t BASE_INTERVAL; // in milliseconds, loop run by Timer2 to set baseLoopFlag. Now it is just interval of the main function
//	
//	static volatile bool baseLoopFlag; // Timer change this variable to true if base loop have to be executed
//	
//	// Now do not used
//	//static volatile uint32_t baseLoopCounter; // Timer increment it. It is reset by runTasker() when reach main function
//	//static volatile uint32_t mainLoopCounter;
//	
//	struct
//	{
//		bool systemOverloaded = false; // true when function doesn't finished when Timer ticked again
//		bool systemUnstable = false; // true forever when at least once systemOverloadedFlag will be true
//	}flag;
//	
//	uint32_t mainTaskDuration = 0; // main function end time minus next main function start time
//
//	Task mainTask; // the main task
//	
//	friend void baseLoopTimerHandler(); // executed by Timer every BASE_INTERVAL
//};


#endif



/*
	To do:
		- 
*/

