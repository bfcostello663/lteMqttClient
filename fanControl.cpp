#include <iostream>
#include <unistd.h>
#include <cstdlib>
		
#include "mqtt/async_client.h"
#include "mqttSettings.h"

const std::string CLIENT_ID("fan-control");

class FanControlCb : public virtual mqtt::callback {
	private:
		int thermastat;
		int temparature;
		std::mutex &mutex;
	public:
		virtual void connection_lost(const std::string& cause) override {
			std::cout << "Connection lost for " << CLIENT_ID << ": " << cause << std::endl;
			mutex.unlock();
		}

		virtual void message_arrived(mqtt::const_message_ptr msg) override {
			try {
				// Do not do range checking for the temparature here.  These will be bounded by the publisher
				int newTemp = std::stoi(msg->to_string());
				if (newTemp != temparature) {
					temparature = newTemp;
					if (temparature >= thermastat) {
						std::cout << "Current Temparature is " << temparature << "F and thermastat set to " << thermastat << "F - fan on\n";
					} else {
						std::cout << "Current Temparature is " << temparature << "F and thermastat set to " << thermastat << "F - fan off\n";
					}
				}
			} catch(...) {
				// if the message is not an integer, then exit
				mutex.unlock();
			}
		}

		FanControlCb(int therm, std::mutex &m) : thermastat(therm), temparature(-1), mutex(m) {}

};

static int body(mqtt::async_client &client, mqtt::connect_options &connOpts, const int thermastat) {
	bool fanOn = false;

	std::mutex mutex;
	mutex.lock();

	FanControlCb cb(thermastat, mutex);
	client.set_callback(cb);

	client.connect(connOpts)->wait();
	std::cout << CLIENT_ID << ": Connected to MQTT broker" << std::endl;

	client.subscribe(mqttSettings::TOPIC, mqttSettings::QOS)->wait();
	std::cout << CLIENT_ID << ": Subscribed to topic: " << mqttSettings::TOPIC << std::endl;

	mutex.lock();

	client.unsubscribe(mqttSettings::TOPIC)->wait();
	std::cout << CLIENT_ID <<": Unsubscribed from topic: " << mqttSettings::TOPIC << std::endl;

	client.disconnect()->wait();
	std::cout << CLIENT_ID << ": Disconnected from MQTT broker" << std::endl;

	return 0;
}


int main(int argc, char *argv[]) {
	int thermastat = 70;
	int option;

	while((option = getopt(argc, argv, "t:")) != -1) {
		// dont waste time on a help messge since this will be called from another binary
		switch(option) {
			case 't':
				int temp = std::atoi(optarg);
				if ((temp >= 60) || (temp <= 90)) {
					thermastat = temp;
				} else {
					std::cout << temp << " out of range (60-90).  Using the default of " << thermastat << std::endl;
				}
		}
	}

	mqtt::async_client client(mqttSettings::SERVER_ADDRESS, CLIENT_ID);

	mqtt::connect_options connOpts;
	connOpts.set_keep_alive_interval(60);
	connOpts.set_clean_session(true);
	connOpts.set_user_name(mqttSettings::USERNAME);
	connOpts.set_password(mqttSettings::PASSWORD);


	return body(client, connOpts, thermastat);
}
