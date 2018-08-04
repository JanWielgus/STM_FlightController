/*	Author: Jan Wielgus
 *	Date: 10.08.2017r.
 *	
	Pierwsza wersja kontrolera lotu drona.
	Funkcje i mo¿liwoœci:
	- Sterowanie w trybie stabilizacji bez wspomagania
	- Komunikacja poprzez bibliotekê PacketSerial
	- Biblioteki obs³ugi akcelerometru, ¿yroskopu, barometru i magnetometru - Korneliusz Jarzebski
 */

#include "config.h"
#include "Communication.h"
#include "Sensors.h"
#include "Motors.h"
#include "PID.h"


//  =====   ZMIENNE   =====

uint32_t last_loop_time = 0; // czas ostatniego wykonania funkcji komunikacji

float headingToHold = 0; // kat do utrzymania przez drona



//  =====   FUNKCJE   =====

inline void updateCommunication();   // aktualizacja komunikacji w odpowiednim czasie
inline void stabilize();             // stabilizacja drona i utrzymywanie kierunku
void configureESC();          // Tryb konfiguracji ESC
void updatePIDParams();       // Aktualizacja parametrów PID odebranymi danymi


PID levelX_PID;
PID levelY_PID;
PID heading_PID;

uint32_t lastLoopTime=0;



void setup()
{
	pinMode(redDiodePin, OUTPUT);
	pinMode(blueDiodePin, OUTPUT);
	digitalWrite(redDiodePin, HIGH);
	digitalWrite(blueDiodePin, LOW);
	
	com.init();                  // inicjalizacja komunikacji
	sensors.init();              // inicjalizacja wszystkich czujników
	motors.init();               // inicjalizacja silników

	updatePIDParams(); // ustawia takie parametry PID jakie sa ustawione w konstruktorze klasy komunikacji
	//Serial.begin(115200);
	
	// Blink
	delay(200);
	digitalWrite(redDiodePin, LOW);
	digitalWrite(blueDiodePin, HIGH);
	delay(200);
	digitalWrite(blueDiodePin, LOW);
}


void loop()
{
	if (micros()-lastLoopTime >= 4000) // 250Hz
	{
		//Serial.println(1000000.0/(micros()-lastLoopTime)); // Ilosc razy na sekunde
		updateCommunication();
		stabilize();
	
		/*
		sensors.readAngles();
		sensors.readCompass();
		Serial.print(sensors.angle.pitch);
		Serial.print("\t");
		Serial.print(sensors.angle.roll);
		Serial.print("\t");
		Serial.println(sensors.headnigGyroMagn);
		*/
		if (com.connectionState())
			digitalWrite(redDiodePin, HIGH);
		else
			digitalWrite(redDiodePin, LOW);
		
		lastLoopTime = micros();
	}
}


////////////////////////////////////////////////////////////////////// ====== END LOOP ======


// Funkcje trzeba wywo³ywaæ ca³y czas. Funkcja odbiera ustawion¹ iloœæ razy na sekunde.
inline void updateCommunication()
{
	if ((millis() - last_loop_time) > COMMUNICATION_WAIT_TIME)
	{
		com.odbierz();
		
		// Jeœli odebrano ramkê z parametrami -> zaktualizuj parametry
		if (com.pidParamsReceivedFlag) updatePIDParams();
		
		if (com.connectionState())
		{
			if (com.armState < 50)
			{
				motors.setMotors(IDLE);
				headingToHold = sensors.headnigGyroMagn; // po uzbrojeniu trzymaj kat jaki byk podczas uzbrajania
			}
			else motors.setMotors(ARMED); // silniki uzbrojone
		}
		else // ZAWSZE rozbrajaj jesli nie ma polaczenia !!!
		{
			motors.setMotors(IDLE);
		}
		
		com.wyslij(DRON_RAMKA_VER1_TYPE);
		
		
		last_loop_time = millis();
	}
}



inline void stabilize()
{
	sensors.readAngles();
	sensors.readCompass();
	
	// Zaktualizuj dt dla calej klasy PID
	PID::updateDeltaTime();
	
	headingToHold += (float(com.pilot.rotate)*PID::getDeltaTime()); // obliczanie kata do utrzymania na podstawie wartosci drazka
	
	// PIDy poziomowania
	int32_t pidX = levelX_PID.get_pid(sensors.angle.pitch + (com.pilot.tilt_TB/10)); // zmienic tu na - jesli leci w zla strone
	int32_t pidY = levelY_PID.get_pid(sensors.angle.roll + (com.pilot.tilt_LR/10));
	
	// PID yaw
	static float headingError;
	headingError = headingToHold-sensors.headnigGyroMagn;
	if (headingError >= 180) headingError -= 360;
	else if (headingError <= -180) headingError += 360;
	int32_t pidHead = heading_PID.get_pid(headingError);
	
	motors.setOnTL(com.pilot.throttle + pidX - pidY - pidHead);
	motors.setOnTR(com.pilot.throttle + pidX + pidY + pidHead);
	motors.setOnBR(com.pilot.throttle - pidX + pidY - pidHead);
	motors.setOnBL(com.pilot.throttle - pidX - pidY + pidHead);
	//motors.executeMotorsNow();
}



void configureESC() // u¿ywane do kalibracji ESC gdy trzeba wszystkie ustawiæ na max/min
{
	motors.setMotors(ARMED);
	
	motors.setOnTL(com.pilot.throttle);
	motors.setOnTR(com.pilot.throttle);
	motors.setOnBR(com.pilot.throttle);
	motors.setOnBL(com.pilot.throttle);
}



void updatePIDParams()
{
	levelX_PID.kP(com.conf.levelPID.kP.value);
	levelX_PID.kI(com.conf.levelPID.kI.value);
	levelX_PID.kD(com.conf.levelPID.kD.value);
	levelX_PID.imax(com.conf.levelPID.Imax);
	levelY_PID.kP(com.conf.levelPID.kP.value);
	levelY_PID.kI(com.conf.levelPID.kI.value);
	levelY_PID.kD(com.conf.levelPID.kD.value);
	levelY_PID.imax(com.conf.levelPID.Imax);
	heading_PID.kP(com.conf.yawPID.kP.value); // 3
	heading_PID.kI(com.conf.yawPID.kI.value); // 1
	heading_PID.kD(com.conf.yawPID.kD.value);
	heading_PID.imax(com.conf.yawPID.Imax);
	
	com.pidParamsReceivedFlag = false; // Juz odebrano dlatego false
}


