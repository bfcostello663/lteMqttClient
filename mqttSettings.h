#ifndef MQTTSETTINGS_H_
#define MQTTSETTINGS_H_

namespace mqttSettings {

	const std::string SERVER_ADDRESS("tcp://broker.emqx.io:1883");
	const std::string USERNAME("emqx");
	const std::string PASSWORD("lteIV");
	const int QOS = 2;
	const std::string TOPIC("cPlusPlus/client/lte/temperature");

	const int MINTEMP = 60;
	const int MAXTEMP = 90;


};


#endif // MQTTSETTINGS_H_
