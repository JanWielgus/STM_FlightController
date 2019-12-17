/*
 * config.h
 *
 * Created: 01/07/2019 12:03:17
 *  Author: Jan Wielgus
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_


namespace config
{
	// Pins
	struct
	{
		uint8_t redDiode = 5;
		uint8_t greenDiode = 6;
		
		// steering
		uint8_t throttle = A0;
		uint8_t rotate = A1;
		uint8_t tiltTB = A2;
		uint8_t tiltLR = A3;
		
		// switches
		uint8_t leftSwitch = 3;
		uint8_t rightSwitch = 4;
		
		
		// radio module
		uint8_t m0pin = 7;
		uint8_t m1pin = 8;
		uint8_t aux = 11;
		
		// bluetooth
		uint8_t btRX = 10;
		uint8_t btTX = 9;
	} const pin;
	
	
	// Addresses
	const uint8_t LCD_ADDRESS = 0x3F;
	
	
	// Stick default border values (maximum tilts)
	struct
	{
		// throttle
		uint16_t thrMin = 84;
		uint16_t thrCen = 84; // throttle center is the same as the min value
		uint16_t thrMax = 965;
		// rotate
		uint16_t rotMin = 50;
		uint16_t rotCen = 515; // center
		uint16_t rotMax = 970;
		// TB tilt
		uint16_t TB_Min = 37;
		uint16_t TB_Cen = 439;
		uint16_t TB_Max = 835;
		// LR tilt
		uint16_t LR_Min = 65;
		uint16_t LR_Cen = 521;
		uint16_t LR_Max = 980;
	} sticksCalibVal;
	
	const uint8_t stickDeadZone = 20;
}





#endif /* CONFIG_H_ */

