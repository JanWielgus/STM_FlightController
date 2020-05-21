/*
    Created:	19/05/2020
    Author:     Jan Wielgus
*/


#include "TaskerFunctions.h"
#include "WiFiCommunication.h"
#include <ITransferable.h>
#include <FC_Communication_Base.h>
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



void setup()
{
    Serial.begin(57600);
    delay(1000);

    pinMode(LED_BUILTIN, OUTPUT);

    // Add all takser functions
    addTaskerFunctionsToTasker();

    // Connect with wifi asynchronically
    Storage::wifiComm.connectWithWifiAsync(&Storage::taskPlanner);
}


void loop()
{
    Storage::tasker.run();
    Storage::taskPlanner.runPlanner();
}
