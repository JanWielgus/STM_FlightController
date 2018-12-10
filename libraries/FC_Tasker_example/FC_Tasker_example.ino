// Author: Jan Wielgus
// Date: 23.11.2018
//
// Example file of FC_Tasker usage.
//

#include "FC_Tasker.h"

void func1(); // main
void func2();

FC_Tasker Tasker(func1, 4000L, 15); // main function
// ! Base interval is something like Tasker time resolution !



void setup()
{
	//Tasker.addMainFunction(func1, 4000L, 10); // 250 Hz - ! NOW in the constructor
	Tasker.addFunction(func2, 500000L, 15); // 2 Hz
	// add other functions
	
	Tasker.scheduleTasks();
}


void loop()
{
	Tasker.runTasker();
}



void func1()
{
	// Function 1 - main
}

void func2()
{
	// Function 2
}
