// 
// 
// 

#include "Storage.h"

namespace Storage
{
	FC_ObjectTasker tasker(config::MaxAmtOfTaskerTasks);
	FC_TaskPlanner taskPlanner(config::MaxAmtOfTaskPlannerTasks);
	FC_Communication_Base comm(&Serial, config::MaxCommPacketBytes);
	WiFiCommunication& wifiComm = WiFiCommunication::getInstance();
}

