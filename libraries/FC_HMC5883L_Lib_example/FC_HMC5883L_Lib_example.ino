/*
    Name:       FC_HMC5883L_Lib_example.ino
    Created:	06/06/2019 14:05:19
    Author:     Jan Wielgus
*/

#include "FC_HMC5883L_Lib.h"


FC_HMC5883L_Lib compass;



void setup()
{
	Serial.begin(9600);
	Serial.println("Program has just started!");
	
	compass.enableHMC_on_MPU(); // IMPORTANT if use GY-86. If not then comment
	
	while (!compass.initialize())
	{
		// If program is there, some problems occured
		Serial.println("HMC5883L cannot be initialized!");
		delay(500);
	}
	
	compass.calibrateCompass();
}


void loop()
{
	compass.readCompassData();
	Serial.print("Heading: ");
	Serial.print(compass.getHeading());
	Serial.println();
	
	delay(14); // ~75Hz
}
