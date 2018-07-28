/*
 * config.h
 *
 * Created: 27.10.2017 18:55:44
 *  Author: Jan Wielgus
 *
 *	Plk konfiguracji wszystkich parametrów drona
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_


//<<<<<<<<<=====================     COMMUNICATION     =====================>>>>>>>>>

	//-------Pamiêtaj o zmianie wielkoœci ramek po zmianie protoko³u!!!-------
	
	//      ===  TYLKO TABLICE O WYMIARACH TAKICH JAK RAMKA, NIE WIÊKSZE !!!  ===
	
	#define serialPort 0	           //Serial: 0, Serial1: 1, Serial2: 2, Serial3: 3
	#define BAUD_RATE 9600
	#define COMMUNICATION_PER_SECOND 20     // Iloœæ wywo³ania funkcji komunikacji w ci¹gu sekundy
	#define MAX_SEND_SIZE 25           // Wielkosæ najwiêkszej ramki

	// Szablon: nadawca_RAMKA_nazwa_SIZE/TYPE
	
	// --  SEND  --
		// Wersja 1 - rozszerzona (pelna)
		#define PILOT_RAMKA_VER1_SIZE 26
		#define PILOT_RAMKA_VER1_TYPE 0x00
	
		// Wersja 2 - podstawowa (narazie bez)
		//#define PILOT_RAMKA_VER2_SIZE x
		//#define PILOT_RAMKA_VER1_TYPE 0x01
	
		// Wersja 3 - parametry PID
		#define PILOT_RAMKA_VER3_SIZE 41
		#define PILOT_RAMKA_VER3_TYPE 0x02
	
	// --  RECEIVE  --
		// Wersja 1 - rozszerzona (pelna)
		#define DRON_RAMKA_VER1_SIZE 32
		#define DRON_RAMKA_VER1_TYPE 0x00
	
		// Wersja 2 - podstawowa (narazie bez)
		//#define DRON_RAMKA_VER2_SIZE x
		//#define DRON_RAMKA_VER2_TYPE 0x01
	
		// Wersja 3 - po uzbrojeniu (takze PID request)
		#define DRON_RAMKA_VER3_SIZE 20
		#define DRON_RAMKA_VER3_TYPE 0x02

	#define MAX_LOST_PACKETS 4          // If more -> signal lost mode
	
	
	// transmission powers
	#define DBM20 4  // 20dBm
	#define DBM14 3  // 14dBm
	#define DBM8 2  // 8dBm
	#define DBM2 1  // 2dBm


//<<<<<<<<<=====================     SENSORS     =====================>>>>>>>>>

	#define PITCH_OFFSET 0   // Offset osi x
	#define ROLL_OFFSET  0   // Offset osi y
	
	#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
	
	#define MAG_X_OFFSET -41
	#define MAG_Y_OFFSET -105
	#define MAG_COMP_ANGLE 70 // kat +/- od/do ktorego jest przeprowadzana kompensacja przechylenia akcelerometrem
	
	#define GYRO_Z_OFFSET 43.15


//<<<<<<<<<=====================     MOTORS     =====================>>>>>>>>>

	#define TLmotorPin 5      // Top left
	#define TRmotorPin 6      // Top right
	#define BRmotorPin 9      // Back left
	#define BLmotorPin 10     // Back right
	
	#define MOTOR_MIN 1000    // Minimum value on the motor
	#define MOTOR_MAX 2000    // Maximum value on the motor
	#define MOTOR_IDLE 1000   // Value when motor is idle


//<<<<<<<<<=====================     PID     =====================>>>>>>>>>

	#define D_error_divisor 4 // dzielnik obliczonego uchybu
	
	
//<<<<<<<<<=====================     AUTO CALCULATIONS     =====================>>>>>>>>>
	
	#define COMMUNICATION_WAIT_TIME (int(1000/COMMUNICATION_PER_SECOND)-2)
	


#endif /* CONFIG_H_ */

