// Author: Jan Wielgus
// Date: 23.11.2018
//
// Example file of FC_Tasker usage.
//

#include "FC_Tasker.h"

void func1(); // main
void func2();


// !WARNING! Only one: FC_SimpleTasker or FC_Tasker can be used at the same time !!!
//FC_SimpleTasker() sTasker;
FC_Tasker tasker(func1, 4000L, 15); // main function



void setup()
{
	//Tasker.addMainFunction(func1, 4000L, 10); // 250 Hz - ! NOW in the constructor
	tasker.addFunction(func2, 500000L, 15); // 2 Hz
	// add other functions
	
	tasker.scheduleTasks();
}


void loop()
{
	tasker.runTasker();
}



void func1()
{
	// Function 1 - main
}

void func2()
{
	// Function 2
}
