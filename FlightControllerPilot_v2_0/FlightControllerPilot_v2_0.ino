#include "ControlSticks.h"
/*
    Created:	01/07/2019
    Author:     Jan Wielgus
*/

#include "FC_Tasker/FC_Tasker.h"
#include ""



void setup()
{
	// Communication serial
	Serial.begin(9600);


	ctrlStk::stickInitiazlize();
}


void loop()
{


}
