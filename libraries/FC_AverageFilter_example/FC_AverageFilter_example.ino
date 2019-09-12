/*
    Name:       FC_AverageFilter_example.ino
    Created:	11/09/2019 19:38:17
    Author:     Jan Wielgus
*/

#include "FC_AverageFilter.h"

FC_AverageFilter<uint16_t, uint32_t, float> simpleAverageFilter(20);
// <sample type, sum type, return type>   (averaged samples)


void setup()
{
	Serial.begin(115200);
	Serial.println("Program has started.");
}


void loop()
{
	uint16_t rawValue = analogRead(A0);
	simpleAverageFilter.addNewSample(rawValue);
	Serial.print("Raw: ");
	Serial.print(rawValue);
	Serial.print(" Averaged: ");
	Serial.println(simpleAverageFilter.getAverage());
	
	delay(50); // 20Hz
}
