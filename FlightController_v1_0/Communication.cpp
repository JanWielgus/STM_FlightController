// Created: 11.01.2018r.
// Author: Jan Wielgus
// 

#include "Communication.h"

CommunicationClass com;

void _protezaOdbierz(const uint8_t* buffer, size_t _size)
{
	com.odbierzPriv(buffer, _size);
}



CommunicationClass::CommunicationClass()
{
	//defaults:
	lost_packets = 10; // domyslnie nie ma polacznenia
	
	pilot.throttle = 0;
	flightMode = 0;
	armState = 0; // x<50 - jest rozbrojony
	signalLostScenario = 2;
	for (int i=0; i<6; i++) cellVoltage[i] = 0;
	pid_params_request = true;
	
	// domyslne parametry PID
	conf.levelPID.kP.value = 0;
	conf.levelPID.kI.value = 0;
	conf.levelPID.kD.value = 0;
	conf.levelPID.Imax = 0;
	//--
	conf.yawPID.kP.value = 0;
	conf.yawPID.kI.value = 0;
	conf.yawPID.kD.value = 0;
	conf.yawPID.Imax = 0;
	//--
	conf.altHoldPID.kP.value = 0;
	conf.altHoldPID.kI.value = 0;
	conf.altHoldPID.kD.value = 0;
	conf.altHoldPID.Imax =0;
}



void CommunicationClass::init()
{
	pSerial.setPacketHandler(_protezaOdbierz);
	pSerial.begin(BAUD_RATE, serialPort);
	
	// transciever deafult settings
	head        = 0xC2;
	addh        = 0x00;
	addl        = 0x00;
	sped.bajt   = 0x18;
	chan.bajt   = 0x50;
	option.bajt = 0x44;
}



void CommunicationClass::odbierz()
{
	if_odbierzPrivFlag = false;
	
	pSerial.update();
	
	// check if connection lost
	if (if_odbierzPrivFlag)
		lost_packets=0;
	else
		lost_packets++;

	if (lost_packets == MAX_LOST_PACKETS) // czas utraty polaczenia
		con_lost_time = millis();
}



void CommunicationClass::odbierzPriv(const uint8_t* bufferR, size_t PacketSize)
{
	// VER 1 - pelna
	if (bufferR[1]==PILOT_RAMKA_VER1_TYPE && PacketSize==PILOT_RAMKA_VER1_SIZE && sprawdzSumeKontr(bufferR, PacketSize))
	{
		pilot.throttle = word(bufferR[3], bufferR[2]);
		pilot.rotate = word(bufferR[5], bufferR[4]);
		pilot.tilt_TB = word(bufferR[7], bufferR[6]);
		pilot.tilt_LR = word(bufferR[9], bufferR[8]);
		distanceFromPilot = word(bufferR[11], bufferR[10]);
		directionToPilot = word(bufferR[13], bufferR[12]);
		flightMode = bufferR[14];
		armState = bufferR[15];
		randomRxValue = bufferR[16];
		bitsRx1.bajt = bufferR[17];
		bitsRx2.bajt = bufferR[18];
		signalLostScenario = bufferR[19];
		// zapas 6 x uint8 (ostatnie [25])
		
		if_odbierzPrivFlag = true;
	}
	
	/*
		VER 2 - podstawowa
	*/
	
	// VER 3 - po uzbrojeniu i PID request
	else if (bufferR[1]==PILOT_RAMKA_VER3_TYPE && PacketSize==PILOT_RAMKA_VER3_SIZE && sprawdzSumeKontr(bufferR, PacketSize))
	{
		// leveling
		for (int i=0; i<4; i++)
			conf.levelPID.kP.bajt[i] = bufferR[i+2];
		for (int i=0; i<4; i++)
			conf.levelPID.kI.bajt[i] = bufferR[i+6];
		for (int i=0; i<4; i++)
			conf.levelPID.kD.bajt[i] = bufferR[i+10];
		conf.levelPID.Imax = bufferR[14];
		
		// yaw
		for (int i=0; i<4; i++)
			conf.yawPID.kP.bajt[i] = bufferR[i+15];
		for (int i=0; i<4; i++)
			conf.yawPID.kI.bajt[i] = bufferR[i+19];
		for (int i=0; i<4; i++)
			conf.yawPID.kD.bajt[i] = bufferR[i+23];
		conf.yawPID.Imax = bufferR[27];
		
		// alt hold
		for (int i=0; i<4; i++)
			conf.altHoldPID.kP.bajt[i] = bufferR[28];
		for (int i=0; i<4; i++)
			conf.altHoldPID.kI.bajt[i] = bufferR[32];
		for (int i=0; i<4; i++)
			conf.altHoldPID.kD.bajt[i] = bufferR[36];
		conf.altHoldPID.Imax = bufferR[40];
		
		// zapas 5 x uint8
		
		// if_odbierzPriv = true; // Tu raczej nie ma znaczenia, musi byc odebrana VER1
	}
}



void CommunicationClass::wyslij(uint8_t typRamki)
{
	buforT[1] = typRamki;
	
	// VER1 - pelna
	if (typRamki == DRON_RAMKA_VER1_TYPE)
	{
		for (int i=0; i<6; i++)
			buforT[i+2] = uint8_t(cellVoltage[i]*10.0);
		buforT[8] = pitch;
		buforT[9] = roll;
		buforT[10] = lowByte(heading);
		buforT[11] = highByte(heading);
		buforT[12] = lowByte(altitude);
		buforT[13] = highByte(altitude);
		for (int i=0; i<4; i++)
			buforT[i+14] = pos_longInt.bajt[i];
		for (int i=0; i<4; i++)
			buforT[i+18] = pos_latInt.bajt[i];
		buforT[22] = randomRxValue;
		buforT[23] = errorList1.bajt;
		buforT[24] = errorList2.bajt;
		buforT[25] = bitsTx1.bajt;
		for (int i=0; i<6; i++) // zerowanie zapasu
			buforT[i+26] = 0;
		// zapas 6 x uint8 (ostatnie [31])
		
		buforT[0] = liczSumeKontr(buforT, DRON_RAMKA_VER1_SIZE);
		pSerial.send(buforT, DRON_RAMKA_VER1_SIZE);
	}
	
	/*
		VER2 - podstawowa
	*/
	
	// VER3 - parametry PID
	else if (typRamki == DRON_RAMKA_VER3_TYPE)
	{
		for (int i=0; i<4; i++)
			buforT[i+2] = takeoff.posLongInt.bajt[i];
		for (int i=0; i<4; i++)
			buforT[i+6] = takeoff.posLatInt.bajt[i];
		for (int i=0; i<4; i++)
			buforT[i+10] = takeoff.pressure.bajt[i];
			
		for (int i=0; i<5; i++) // zerowanie zapasu
			buforT[i+14] = 0;
		// zapas 5 x uint8
		
		buforT[19] = pid_params_request;
		
		buforT[0] = liczSumeKontr(buforT, DRON_RAMKA_VER3_SIZE);
		pSerial.send(buforT, DRON_RAMKA_VER3_SIZE);
	}
}



bool CommunicationClass::connectionState()
{
	if (lost_packets < MAX_LOST_PACKETS)
		return true;
	timeAfterSL = millis()-con_lost_time;
	return false; // otherwise
}



bool CommunicationClass::sprawdzSumeKontr(const uint8_t* buffer, size_t PacketSize)
{
	uint8_t suma_kontrolna = buffer[1];
	for(int i=2; i<PacketSize; i++)
	suma_kontrolna ^= buffer[i];	//xor'owanie kolejnych bajt?w

	if(suma_kontrolna==buffer[0]) return true;
	else return false;
}



uint8_t CommunicationClass::liczSumeKontr(const uint8_t* buffer, size_t PacketSize)
{
	uint8_t suma_kontrolna = buffer[1];
	for(int i=2; i<PacketSize; i++)
	suma_kontrolna ^= buffer[i];	//xor'owanie kolejnych bajt?w

	return suma_kontrolna;
}



void CommunicationClass::setTransmitPower(bool b1, bool b2)
{
	if (b1 == 1 && b2 == 1)
	{
		// 18dBm
		option.b6 = 1;
		option.b7 = 1;
	}
	
	else if (b1 == 1 && b2 == 0)
	{
		// 21dBm
		option.b6 = 1;
		option.b7 = 0;
	}
	
	else if (b1 == 0 && b2 == 1)
	{
		// 24dBm
		option.b6 = 0;
		option.b7 = 1;
	}
	
	else
	{
		// 27dBm
		option.b6 = 0;
		option.b7 = 0;
	}
	changeInTxParamsFlag = true; // send new params to module
}



// overloaded
void CommunicationClass::setTransmitPower(uint8_t power)
{
	switch (power)
	{
		case DBM2:
			setTransmitPower(1, 1);
			break;
		case DBM8:
			setTransmitPower(1, 0);
			break;
		case DBM14:
			setTransmitPower(0, 1);
			break;
		case DBM20:
			setTransmitPower(0, 0);
			break;
		default:
			setTransmitPower(0, 0);
	}
}



// overloaded
void CommunicationClass::setTransmitPower()
{
	if (connectionState())
	{
		if (bitsRx2.b0 == 0) // jesli moc ustawia pilot
		{
			// requested transmission power
			setTransmitPower(bitsRx2.b2, bitsRx2.b1);
		}
		else
			autoTransmitPower();
	}
	else
	{
		// MAX when signal lost
		setTransmitPower(DBM20); // MAX
	}
}



void CommunicationClass::setOTASpeed(bool b1)
{
	if (b1)
	{
		// 2.4kbps
		sped.b5 = 0;
		sped.b6 = 0;
		sped.b7 = 1;
	}
	
	else
	{
		// 1.2kbps
		sped.b5 = 0;
		sped.b6 = 0;
		sped.b7 = 0;
	}
	changeInTxParamsFlag = true;
}


void CommunicationClass::autoTransmitPower()
{
	// Do napisania !!!
	
	setTransmitPower(DBM20); // temp
}



void CommunicationClass::writeParamsToTransceiver()
{
	if (changeInTxParamsFlag)
	{
		// write if was change in parameters
		//...
		
		changeInTxParamsFlag = false;
	}
	return;
}



void CommunicationClass::getTransceiverParams()
{
	/*
	// power
	settingsConfirmation.b7 = switchesR.b6;
	settingsConfirmation.b6 = switchesR.b5;
	
	// working modes
	settingsConfirmation.b5 = switchesR.b4;
	settingsConfirmation.b4 = switchesR.b3;
	
	// OTA speed
	settingsConfirmation.b3 = switchesR.b2;
	*/
}
