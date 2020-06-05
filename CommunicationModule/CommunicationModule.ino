/*
    Created:	19/05/2020
    Author:     Jan Wielgus
*/


#include "TaskerFunctions.h"
#include <ITransferable.h>
#include <DataPacketBase.h>
#include <FC_TaskPlanner.h>
#include <FC_Task.h>
#include <FC_ObjectTasker.h>
#include "Storage.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <FC_GrowingArray.h>
#include <FC_CustomDataTypes.h>
#include <FC_EVA_Filter.h>
#include "FC_ESP8266_WiFiComm.h"
#include <FC_SerialCommBase.h>



void setup()
{
    Serial.begin(57600);
    delay(1000);

    // setup low-level serial comm
    Storage::serialComm.begin();

    // setup low-level wifi comm
    Storage::WiFiComm.begin();
    Storage::WiFiComm.setTargetIPAddrAlwaysToLastSender();

    pinMode(LED_BUILTIN, OUTPUT);

    // Add all takser functions
    addTaskerFunctionsToTasker();


}


void loop()
{
    Storage::tasker.run();
}
