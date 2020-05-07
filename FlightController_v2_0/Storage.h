/*
 * Storage.h
 *
 * Created: 21/07/2019 17:45:28
 *  Author: Jan Wielgus
 *
 *
 * There are all global variables and objects
 */ 


#ifndef STORAGE_H_
#define STORAGE_H_


#include <FC_ObjectTasker.h>
#include <FC_Task.h>
#include <FC_TaskPlanner.h>
#include <MyPID.h>
#include <FC_Communication_Base.h>
#include <FC_MPU6050Lib.h>
#include <FC_HMC5883L_Lib.h>
#include <FC_MS5611_Lib.h>
#include <FC_EVA_Filter.h>
#include "FC_Motors.h"
#include "DebugSystem.h"
#include "SharedDataTypes.h"
#include "Interfaces.h"
#include <FC_CommunicationHandler.h>
#include "CommRecDataPackets.h"
#include "CommSendDataPackets.h"



// Variables and changing state stuff

namespace Storage
{
    // Flags
    struct
    {
        bool thereIsNoFlags = true;
    } flags;



    // Objects
    extern FC_ObjectTasker tasker;
    extern FC_TaskPlanner taskPlanner;
    extern FC_CommunicationHandler comm;
    extern FC_MPU6050Lib mpu;
    extern FC_HMC5883L_Lib compass;
    extern FC_MS5611_Lib baro;
    extern FC_Motors motors;
    extern DebugSystem debug;


    // PID objects
    extern MyPID levelXpid;
    extern MyPID levelYpid;
    extern MyPID yawPID;
    extern MyPID altHoldPID;


    // VirtualPilot object
    extern IVirtualPilot& virtualPilot;


    // Used flight modes
    extern IFlightMode& stabilizeFlightMode;
    extern IFlightMode& altHoldFlightMode;
    extern IFlightMode& posHoldFlightMode;


    // Global sensor readings
    extern readingsType reading;
    extern virtualSticksType sticksFiltered;

}




#endif /* STORAGE_H_ */

