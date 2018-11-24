// Author: Jan Wielgus
// Date: 23.11.2018
//
// Example file of FC_Tasker usage.
//

#include "FC_Tasker.h"

FC_Tasker Tasker; // or Tasker( base interval )
// ! Base interval is something like Tasker time resolution !

void func1(); // main
void func2();


void setup()
{
	Tasker.addMainFunction(func1, 4000L, 10); // 250 Hz
	Tasker.addFunction(func2, 500000L, 15); // 2 Hz
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
