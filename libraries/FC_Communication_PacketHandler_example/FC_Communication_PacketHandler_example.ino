/*
    Name:       FC_Communication_PacketHandler_example.ino
    Created:	30/05/2019 23:10:11
    Author:     Jan Wielgus
	
	NOTE:
	This code is an example class. For further use there should be made individual class for each purpose
	eg. one for communication and for debugging, each with proper name and each inherit form FC_Communication_Base class.
*/

#include "FC_Communication_PacketHandler.h"

const uint8_t MAX_SEND_SIZE = 30;

FC_Communication_PacketHandler com(&Serial, MAX_SEND_SIZE);



void setup()
{

}



void loop()
{


}
