// Author: Jan Wielgus
// Date: 24.10.2017
// 

#include "Komunikacja.h"
KomunikacjaClass kom;

void _protezaOdbierz(const uint8_t* buffer, size_t size)
{
	kom.odbierzPriv(buffer, size);
}



void KomunikacjaClass::init()
{
	pSerial.setPacketHandler(_protezaOdbierz);
	pSerial.begin(BAUD_RATE, serialPort);
}



void KomunikacjaClass::odbierz()
{
	pSerial.update();
}



void KomunikacjaClass::odbierzPriv(const uint8_t* bufferR, size_t PacketSize)
{
	if (bufferR[1] == PILOT_RAMKA_TEST_TYPE && PacketSize == PILOT_RAMKA_TEST_SIZE && sprawdzSumeKontr(bufferR, PacketSize))
	{
		pilot.throttle  = word(bufferR[3], bufferR[2]);
		pilot.tilt_TB   = bufferR[4];
		pilot.tilt_LR   = bufferR[5];
		pilot.rotate    = bufferR[6];
		pong.bajt       = bufferR[7];
	}
	
	// [...]
	
	else if (bufferR[1] == PILOT_RAMKA_CONFIG_TYPE && PacketSize == PILOT_RAMKA_CONFIG_SIZE && sprawdzSumeKontr(bufferR, PacketSize))
	{
		conf.kP_level.bajt[0]    = bufferR[2];
		conf.kP_level.bajt[1]    = bufferR[3];
		conf.kP_level.bajt[2]    = bufferR[4];
		conf.kP_level.bajt[3]    = bufferR[5];
		
		conf.kI_level.bajt[0]    = bufferR[6];
		conf.kI_level.bajt[1]    = bufferR[7];
		conf.kI_level.bajt[2]    = bufferR[8];
		conf.kI_level.bajt[3]    = bufferR[9];
		
		conf.kD_level.bajt[0]    = bufferR[10];
		conf.kD_level.bajt[1]    = bufferR[11];
		conf.kD_level.bajt[2]    = bufferR[12];
		conf.kD_level.bajt[3]    = bufferR[13];
		
		conf.I_level_limiter     = bufferR[14];
		
		conf.kP_yaw.bajt[0]      = bufferR[15];
		conf.kP_yaw.bajt[1]      = bufferR[16];
		conf.kP_yaw.bajt[2]      = bufferR[17];
		conf.kP_yaw.bajt[3]      = bufferR[18];
		
		conf.kD_yaw.bajt[0]      = bufferR[19];
		conf.kD_yaw.bajt[1]      = bufferR[20];
		conf.kD_yaw.bajt[2]      = bufferR[21];
		conf.kD_yaw.bajt[3]      = bufferR[22];
		
		recievedConfigPacket = true;  // Odebrano pakiet konfiguracyjny
	}
}



void KomunikacjaClass::wyslij(uint8_t typRamki)
{
	buforT[1] = typRamki;
	
	if (typRamki == DRON_RAMKA_TEST_TYPE)
	{
		buforT[2] =  zmiennaTestowa1.bajt[0];
		buforT[3] =  zmiennaTestowa1.bajt[1];
		buforT[4] =  zmiennaTestowa1.bajt[2];
		buforT[5] =  zmiennaTestowa1.bajt[3];
		
		buforT[0] = liczSumeKontr(buforT, DRON_RAMKA_TEST_SIZE);
		
		pSerial.send(buforT, DRON_RAMKA_TEST_SIZE);
	}
	
	else if (typRamki == DRON_RAMKA_POZYCJA_TYPE)
	{
		/*
		buforT[2] = jakisOsiemTtx;
		buforT[3] = jakisBitByteTx;
		*/
	}
}



void KomunikacjaClass::updateSignal()
{
	if (pong.b0 != ostatni_pong)
	{
		ostatni_pong = pong.b0;
		zgubione_ramki = 0;
	}
	else
	{
		zgubione_ramki++;
	}
}



bool KomunikacjaClass::isSignal()
{
	if (zgubione_ramki > MAX_ILOSC_ZGUBIONYCH_RAMEK)
		return false;
	return true;
}



bool KomunikacjaClass::sprawdzSumeKontr(const uint8_t* buffer, size_t PacketSize)
{
	uint8_t suma_kontrolna = buffer[1];
	for(int i=2; i<PacketSize; i++)
	suma_kontrolna ^= buffer[i];	//xor'owanie kolejnych bajt?w

	if(suma_kontrolna==buffer[0]) return true;
	else return false;
}



uint8_t KomunikacjaClass::liczSumeKontr(const uint8_t* buffer, size_t PacketSize)
{
	uint8_t suma_kontrolna = buffer[1];
	for(int i=2; i<PacketSize; i++)
	suma_kontrolna ^= buffer[i];	//xor'owanie kolejnych bajt?w

	return suma_kontrolna;
}



