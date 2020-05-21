#ifndef CONFIG_H_
#define CONFIG_H_


namespace config
{
	const uint8_t MaxAmtOfTaskerTasks = 15; // tasks array size inside tasker
	const uint8_t MaxAmtOfTaskPlannerTasks = 10; // max amount of tasks planned at once

	const uint8_t MaxCommPacketBytes = 100; // communication buffer size



	struct
	{
		char* SSID = "HUAWEI_P_smart"; // wifi ssid
		char* PASS = "22222222"; // wifi password
		uint16_t port = 8888;
		uint8_t connectingTimeTimeout = 300; // in seconds, max time of trying to connect to wifi
		uint32_t receivingInterval = 4000; // in us, 100000 - 10Hz  (1000000 / hz = interval)
	} const wifiComm;
}

#endif /* CONFIG_H_ */

