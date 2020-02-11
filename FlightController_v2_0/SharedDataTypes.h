/*
 * CustomDataTypes.h
 *
 * Created: 10/02/2020
 *  Author: Jan Wielgus
 *
 *
 * Data types created for the drone program needs
 */


#ifndef CUSTOMDATATYPES_H_
#define CUSTOMDATATYPES_H_



 // list of all flight modes
enum FlightModeType
{
    STABILIZE,
    ALT_HOLD,
    POS_HOLD,
    LAST_FLIGHT_MODE = POS_HOLD // !!! REMEMBER TO SET THE HIGHEST VALUE !!!
};
// Do not use separate flight modes type. ADD ALL FLIGHT MODES TO THE ENUM ABOVE !!!
// {LANDING=3, RETURN_TO_LAUNCH=4, RETURN_OVER_PILOT=5};

enum BaudRates { BAUD_9600 = 9600, BAUD_19200 = 19200, BAUD_38400 = 38400, BAUD_57600 = 57600, BAUD_115200 = 115200 };




// Only for STM32 (maybe not necessary)
typedef uint8 uint8_t;
typedef int8 int8_t;
typedef uint16 uint16_t;
typedef int16 int16_t;




#endif /* STORAGE_H_ */

