// Author: Jan Wielgus
// Date: 23.11.2018
//
// Example file of FC_Tasker usage.
//

/*
	READ THAT!
	Using FC_Tasker, after upload you need to restart the processor!
*/

#include "FC_Tasker.h"

void func1(); // main
void func2();
void func3();


// !!! Only one: FC_SimpleTasker or FC_Tasker can be used at the same time !!! (when using Simple, constructor have no parameters)
FC_SimpleTasker tasker;

// Only SimpleTasker works for now (problem with the timer)
//FC_Tasker tasker(func1, 500000L, 15); // main function, 2Hz



void setup()
{
	Serial.begin(115200);
	Serial.println("Program has started!");
	
	tasker.addFunction(func2, 1000000L, 15); // 1 Hz
	tasker.addFunction(func3, 1000000L, 15); // 1 Hz
	// add other functions
	
	//tasker.scheduleTasks(); // This function is not working for now
	
	Serial.println("End on setup!");
}


void loop()
{
	tasker.runTasker();
}



void func1()
{
	// Function 1 - main
	Serial.println(" F1");
}

void func2()
{
	// Function 2
	Serial.println("F2 test message");
}

void func3()
{
	// Function 3
	Serial.println("F3 -----");
}
