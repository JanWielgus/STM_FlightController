// FC_CustomDataTypes.h
/*
	Created:	03/06/2019
	Author:     Jan Wielgus
*/

#ifndef _FC_CUSTOMDATATYPES_h
#define _FC_CUSTOMDATATYPES_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


class floatByte
{
 private:
	union
	{
		float value;
		uint8_t byte[4];
	}data;

 public:
	floatByte(float v=0)
	{
		data.value = v;
	}

	operator float()
	{
		return data.value;
	}

	uint8_t* byteArr()
	{
		return data.byte;
	}

};



class uint16Byte
{
 private:
	union
	{
		uint16_t value;
		uint8_t byte[2];
	}data;

 public:
	uint16Byte(uint16_t v=0)
	{
		data.value = v;
	}

	operator uint16_t()
	{
		return data.value;
	}

	uint8_t* byteArr()
	{
		return data.byte;
	}
};



class int16Byte
{
 private:
	union
	{
		int16_t value;
		uint8_t byte[2];
	}data;

 public:
	int16Byte(int16_t v=0)
	{
		data.value = v;
	}

	operator int16_t()
	{
		return data.value;
	}

	uint8_t* byteArr()
	{
		return data.byte;
	}
};



union bitByte
{
	bool value[8];
	uint8_t byte;
};





#endif

