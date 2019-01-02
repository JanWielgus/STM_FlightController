// Author: Jan Wielgus
// Date: 02.01.2018
//
// Example file of FC_Motors usage.
//

#include "FC_Motors.h"

FC_Motors motors;


void setup()
{
	motors.setOnTL(0);
	motors.setOnTR(0);
	motors.setOnBL(0);
	motors.setOnBR(0);
	
	delay(5000);
}


void loop()
{
	// set 10% of power
	motors.setOnTL(100);
	motors.setOnTR(100);
	motors.setOnBL(100);
	motors.setOnBR(100);
	
	motors.forceMotorsExecution(); // Generate pulses at the moment
	
	delay(4); // +- 250Hz
}
