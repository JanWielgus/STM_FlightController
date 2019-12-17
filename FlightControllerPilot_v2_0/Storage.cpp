#include "Storage.h"
#include "config.h"


// Objects
FC_SimpleTasker tasker;
FC_MainCommunication com(&Serial, 100);
LiquidCrystal_I2C lcd(config::LCD_ADDRESS, 16, 2);
LcdHandler display;
SoftwareSerial bluetoothSoftwareSerial(config::pin.btRX, config::pin.btTX); // RX, TX
AndroidCommunication androidCom;
GestureRecognizer gestureRecognizer;


// control sticks
FC_ControlStick thrStick;
FC_ControlStick rotStick;
FC_ControlStick TB_Stick;
FC_ControlStick LR_Stick;


// Other types
armStateType armState = disarmed;

