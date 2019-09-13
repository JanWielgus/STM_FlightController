/*
    Name:       FC_MS5611_example.ino
    Created:	02/09/2019 13:06:01
    Author:     Jan Wielgus
*/

#include "FC_MS5611_Lib.h"
#include "FC_Tasker.h"


//FC_MS5611_Lib baro; // Baro object is created inside the library
FC_SimpleTasker tasker;

void showPressure(); // this function show pressure in 80Hz


void setup()
{
	Serial.begin(115200);
	Serial.println("Program has started.");
	
	delay(200);
	
	// Add tasker function to show pressure
	tasker.addFunction(showPressure, 12500, 10); // 80Hz
	
	
	
	while (!baro.initialize())
	{
		// Some problems occured
		Serial.println("Baro cannot be initialized");
		delay(500);
	}
	
	// Set I2C 400kHz clock
	baro.setFastClock();
	
	Serial.println("Setup completed");
}


void loop()
{
	baro.runBarometer();
	tasker.runTasker();
}



void showPressure()
{
	Serial.print("Pressure: ");
	Serial.print(baro.getPressure());
	Serial.println();
}
