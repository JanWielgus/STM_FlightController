#include "Storage.h"
#include "config.h"

using namespace config;


// Objects
FC_SimpleTasker tasker;
FC_MainCommunication com(&Serial1, 45);
FC_MPU6050Lib mpu;
FC_HMC5883L_Lib compass;
// Barometer object is created inside the library
FC_Motors motors;


// PID objects
static const float calculationsDeltaTime = 0.004f; //  = 1/250
MyPID levelXpid(calculationsDeltaTime, defPID.leveling.p, defPID.leveling.i, defPID.leveling.d, defPID.leveling.imax);
MyPID levelYpid(calculationsDeltaTime, defPID.leveling.p, defPID.leveling.i, defPID.leveling.d, defPID.leveling.imax);
MyPID yawPID(calculationsDeltaTime, defPID.yaw.p, defPID.yaw.i, defPID.yaw.d, defPID.yaw.imax);
MyPID altHoldPID(calculationsDeltaTime, defPID.altHold.p, defPID.altHold.i, defPID.altHold.d, defPID.altHold.imax);



// Variables
FC_MPU6050Lib::vector3Float angle; // X and Y angles
float heading;
float headingToHold = 0; // calculated value based on the pilot rotate stick to hold by the drone
float pressureToHold;


// Extrapolation variables
int16_t previousTBvalue = 0;
int16_t previousLRvalue = 0;
float extrapolatedTBstick = 0;
float extrapolatedLRstick = 0;
FC_EVA_Filter tbFilter(0.4);
FC_EVA_Filter lrFilter(0.4);


// Flight modes
// PID controllers results
int16_t pidXval;
int16_t pidYval;
int16_t pidYawVal;
int16_t pidAltHoldVal;

uint16_t altHoldBaseThrottle; // throttle value in which drone should keep its own altitude

