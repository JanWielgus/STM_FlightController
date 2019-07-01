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
		uint8_t redDiode;
		uint8_t greenDiode;
	} pin;

	// Defalut pin values
	pin.redDiode = 5;
	pin.greenDiode = 6;
	
	
	uint8_t LCD_ADDRESS = 0x3F;
}





#endif /* CONFIG_H_ */

