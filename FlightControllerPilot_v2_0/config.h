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
	struct
	{
		uint8_t redDiode = 5;
		uint8_t greenDiode = 6;
		
		// steering
		uint8_t throttle = A0;
		uint8_t rotate = A1;
		uint8_t tiltTB = A2;
		uint8_t tiltLR = A3;
		
	} pin;
/* If does not compile with default values in the structure
	// Defalut pin values
	pin.redDiode = 5;
	pin.greenDiode = 6;
	pin.throttle = A0;
	pin.rotate = A1;
	pin.tiltTB = A2;
	pin.tiltLR = A3;*/
	
	
	// Addresses
	uint8_t LCD_ADDRESS = 0x3F;
	
	
	// Stick default border values (maximum tilts)
	struct
	{
		// throttle
		uint16_t thrMin = 65;
		uint16_t thrMax = 960;
		// rotate
		uint16_t rotMin = 48;
		uint16_t rotCen = 514; // center
		uint16_t rotMax = 970;
		// TB tilt
		uint16_t TB_Min = 16;
		uint16_t TB_Cen = 490;
		uint16_t TB_Max = 900;
		// LR tilt
		uint16_t LR_Min = 65;
		uint16_t LR_Cen = 526;
		uint16_t LR_Max = 984;
	} tiltsRange;
	
	uint8_t stickDeadZone = 10;
}





#endif /* CONFIG_H_ */

