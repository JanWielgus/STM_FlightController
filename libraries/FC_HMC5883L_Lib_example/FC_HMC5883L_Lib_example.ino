/*
    Name:       FC_HMC5883L_Lib_example.ino
    Created:	06/06/2019 14:05:19
    Author:     Jan Wielgus
*/

#include "FC_HMC5883L_Lib.h"


FC_HMC5883L_Lib compass;



void setup()
{
	compass.initialize();
	
	compass.calibrateCompass();
}


void loop()
{
	compass.readCompassData();
	Serial.print("Heading: ");
	Serial.print(compass.getHeading());
	Serial.println();
}
