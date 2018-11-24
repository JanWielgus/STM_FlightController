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

class FC_Tasker
{
 public:
	FC_Tasker(uint16_t base_interval = 500);
	~FC_Tasker();
	void addFunction( void (*funcPointer)(), long interv, uint16_t maxDur );
	void addMainFunction( void (*mainFuncPointer)(), long interv, uint16_t maxDur );
	//void scheduleTasks() // to rethink if it is necessary
	void runTasker(); // should be the only function in loop. Execute tasks in intelligent way, not everything at one time
	

 private:
	const uint16_t BASE_INTERVAL; // in milliseconds, loop run by Timer2 to set baseLoopFlag. It is like a executing resolution
	
	static volatile bool baseLoopFlag; // Timer change this variable to true if base loop have to be executed
	static volatile uint16_t baseLoopCounter; // Timer increment it. It is reset by runTasker() when reach main function
	bool systemOverloadedFlag = false; // true when function doesn't finished when Timer ticked again
	bool systemUnstableFlag = false; // true forever when at least once systemOverloadedFlag will be true
	
	struct Task
	{
		void (*functionPointer)(); // pointer to the function
		long interval; // in milliseconds
		uint16_t maxDuration; // in milliseconds
	};

	Task * mainTask; // pointer to the main task
	Task * taskList; // dynamically created, list of tasks, except main task (it is in other pointer)
	uint8_t numberOfTasks = 0;
	
	void copyTaskList(Task *from, Task *to, uint8_t amount);
	friend void baseLoopTimerHandler(); // executed by Timer every BASE_INTERVAL
};


#endif

