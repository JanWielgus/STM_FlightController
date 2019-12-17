#include "Storage.h"


// Objects
FC_SimpleTasker tasker;
FC_MainCommunication com(&Serial, 100);
LiquidCrystal_I2C lcd(config::LCD_ADDRESS, 16, 2);
SoftwareSerial bluetoothSoftwareSerial(config::pin.btRX, config::pin.btTX); // RX, TX


// control sticks
FC_ControlStick thrStick;
FC_ControlStick rotStick;
FC_ControlStick TB_Stick;
FC_ControlStick LR_Stick;


// Other types
armStateType armState = disarmed;


// Bluetooth app received data
bool btNeedToUpdatePIDFlag = false; // if this is true - need to send new PID data to the drone
// RESET THIS TO false AFTER SENDING TO THE DRONE

uint8_t btControllerID;
float btPID_P;
float btPID_I;
uint8_t btPID_Imax;
float btPID_D;

