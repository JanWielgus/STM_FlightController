/*
    Name:       FC_TaskPlanner_example.ino
    Created:	10/09/2019 21:32:39
    Author:     Jan Wielgus
*/

#include "FC_TaskPlanner.h"

FC_TaskPlanner taskPlanner(5);


void plannedTask1();
void plannedTask2();


void setup()
{
	Serial.begin(115200);
	Serial.println("Program has been started!");
	
	taskPlanner.scheduleTask(plannedTask1, 2000); // call task1 in 2000ms (once)
	Serial.println("Task1 has been planned to execute in 2 seconds");
}


void loop()
{
	taskPlanner.runPlanner();
}



void plannedTask1()
{
	Serial.println("Planned task 1 has been executed");
	
	taskPlanner.scheduleTask(plannedTask2, 3000);
	Serial.println("Task 2 has been planned to execute in 3 seconds");
}


void plannedTask2()
{
	Serial.println("Planned task 2 has been executed");
}
