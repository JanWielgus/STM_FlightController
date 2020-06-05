#ifndef CONFIG_H_
#define CONFIG_H_


namespace config
{
	const uint8_t MaxAmtOfTaskerTasks = 15; // tasks array size inside tasker

	const uint8_t MaxCommPacketSize = 100; // communication buffer size



	struct
	{
		char* SSID = "HUAWEI_P_smart"; // wifi ssid
		char* PASS = "22222222"; // wifi password
		uint16_t port = 8888;
	} const wifiCommConfig;
}

#endif /* CONFIG_H_ */

