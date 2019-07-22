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
		
		
		// radio module
		uint8_t m0pin = 9;
		uint8_t m1pin = 10;
		uint8_t aux = 11;
	} pin;
	
	
	// Addresses
	uint8_t LCD_ADDRESS = 0x3F;
	
	
	// Stick default border values (maximum tilts)
	struct
	{
		// throttle
		uint16_t thrMin = 65;
		uint16_t thrCen = 65; // throttle center is the same as the min value
		uint16_t thrMax = 960;
		// rotate
		uint16_t rotMin = 48;
		uint16_t rotCen = 514; // center
		uint16_t rotMax = 970;
		// TB tilt
		uint16_t TB_Min = 14;
		uint16_t TB_Cen = 487;
		uint16_t TB_Max = 893;
		// LR tilt
		uint16_t LR_Min = 65;
		uint16_t LR_Cen = 526;
		uint16_t LR_Max = 984;
	} tiltsRange;
	
	uint8_t stickDeadZone = 20;
}





#endif /* CONFIG_H_ */

