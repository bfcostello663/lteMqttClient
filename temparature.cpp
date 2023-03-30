#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include "mqtt/async_client.h"
#include "mqttSettings.h"

const std::string CLIENT_ID("temparature-publish");

class TemparatureCb : public virtual mqtt::callback {
	public:
		virtual void connection_lost(const std::string& cause) override {
			std::cout << "Connection lost for " << CLIENT_ID << ": " << cause << std::endl;
			exit(1);
		}


		virtual void message_arrived(mqtt::const_message_ptr msg) override {
			std::cout << "Message arrived on " << CLIENT_ID << " for topic '" << msg->get_topic() << "': " << msg->to_string() << std::endl;
		}

};

static void relayDisconnect(mqtt::async_client &client) {
	auto msg = mqtt::make_message(mqttSettings::TOPIC,
			"q");
	msg->set_qos(mqttSettings::QOS);
	client.publish(msg)->wait();
}


static int body(mqtt::async_client &client, mqtt::connect_options &connOpts) {
	TemparatureCb cb;
	client.set_callback(cb);

	client.connect(connOpts)->wait();
	std::cout << CLIENT_ID << ": Connected to MQTT broker" << std::endl;


	while(1) {
		std::string str;
		std::cout << "Enter the living room temparature in Farenheit (60-90) or q to quit: ";
		std::cin >> str;
		if (str == "q") {
			relayDisconnect(client);
			break;
		}

		try {
			auto temp = std::stoi(str);
			while ((temp < 60) || (temp > 90)) {
				std::cout << temp << " is an invalid teperature\n";
				std::cout << "Enter the living room temparature in Farenheit (60-90) or q to quit: ";
				std::cin >> str;
				if (str == "q") {
					relayDisconnect(client);
					client.disconnect()->wait();
					std::cout << CLIENT_ID << ": Disconnected from MQTT broker" << std::endl;
					return 0;
				}
				temp = std::stoi(str); 
			}

			std::cout << "Send '" << temp << "' to topic '" << mqttSettings::TOPIC << "'\n";
			auto msg = mqtt::make_message(mqttSettings::TOPIC,
					str);
			msg->set_qos(mqttSettings::QOS);
			client.publish(msg)->wait();
		} catch(...) {
			std::cerr << "Invalid input " << str << std::endl;
		}
	}

	client.disconnect()->wait();
	std::cout << CLIENT_ID << ": Disconnected from MQTT broker" << std::endl;
	return 0;	
}

int main() {
	mqtt::async_client client(mqttSettings::SERVER_ADDRESS, CLIENT_ID);

	mqtt::connect_options connOpts;
	connOpts.set_keep_alive_interval(60);
	connOpts.set_clean_session(true);
	connOpts.set_user_name(mqttSettings::USERNAME);
	connOpts.set_password(mqttSettings::PASSWORD);


	return body(client, connOpts);
}	

