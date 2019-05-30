// FC_Communication_PacketHandler.h

#ifndef _FC_COMMUNICATION_PACKETHANDLER_h
#define _FC_COMMUNICATION_PACKETHANDLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <FC_Communication_Base.h>



class FC_Communication_PacketHandler : public FC_Communication_Base
{
 public:
	FC_Communication_PacketHandler::FC_Communication_PacketHandler(Stream* serial, uint8_t bufSize);
 
	struct receivedGardenData
	{
		uint8_t var1;
		int liczba;
		float zmienna;
		int innaLiczba;
	};
	
	struct gardenDataToSend
	{
		float temp;
		int zmiennaDoWyslania;
		uint8_t otherVar;
	};


	// Tu musisz sobie przypomniec jak nadpisac stara metode z klasy podstawowej ale uzyc starej metody we wnetrzu nowej
	// W starej bedzie trzeba dopisac virtual lub zrobic inna nazwe (lub chyba sie da uzyc takiej samej a starej uzywac klasa::metoda() )
	
	// metody: odbieranie, wysylanie, sprawdzanie stanu polaczenia (moze to dac do podstawowej??!), przekazanie spakowanej paczki do wyslania
	
	void getReceivedData(receivedGardenData&); // returns reference to the structure of data to save time
	
	
	
 private:
	
	receivedGardenData receivedData; // this example data is private for this class and can be accessed through the getData() method
	gardenDataToSend dataToSend; // powrorz sobie jak zrobic, zeby odebrane dane byly read-only dla outside a w dataToSend dalo sie wpisac dane przez jakas metode (przeslac gotowa paczke przez referencje)
};


#endif

