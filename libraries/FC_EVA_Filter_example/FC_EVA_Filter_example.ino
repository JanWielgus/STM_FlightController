// Author: Jan Wielgus
// Date: 23.11.2018
//
// Example file of FC_EVA_Filter usage.
// EVA - Exponentially weighted averages
//

#include "FC_EVA_Filter.h"

FC_EVA_Filter filter(0.65);


void setup()
{
	// put your setup code here, to run once:
	Serial.begin(115200);
	Serial.println("Program has started.");
}

void loop() {
  // put your main code here, to run repeatedly:
	int newValue;
	//newValue = analogRead(A1); // THERE PUT NOISY SENSOR READING
	Serial.print("Value: ");
	Serial.print(newValue);
	Serial.print("Filtered: ");
	Serial.println(filter.updateFilter((float&)newValue));
	delay(10);
}
