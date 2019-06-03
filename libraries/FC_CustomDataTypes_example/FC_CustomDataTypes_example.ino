/*
    Name:       FC_CustomDataTypes.ino
    Created:	03/06/2019 13:58:02
    Author:     Jan Wielgus
	
	New data types used in the communication
*/

#include "FC_CustomDataTypes.h"



void setup()
{
	Serial.begin(9600);
	Serial.println("Program has started!");	
}


void loop()
{
	uint16Byte zmienna;
	zmienna = 15;
	Serial.print("zmienna: ");
	Serial.println(zmienna);
	
	uint16Byte testowa;
	testowa.byteArr()[0] = zmienna.byteArr()[0];
	testowa.byteArr()[1] = zmienna.byteArr()[1];
	// teraz testowa powinna miec taka sama wartosc jak zmienna
	Serial.print("testowa: ");
	Serial.println(testowa);
	
	delay(2000);
	
}
