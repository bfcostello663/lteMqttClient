
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sstream>

#include "mqttSettings.h"

int main() {
	int thermostat = 70;

	std::cout << "\nSet the thermostat to a temperature, in Fahrenheit, within the range of " << 
		mqttSettings::MINTEMP << "-" << mqttSettings::MAXTEMP << ":\n";
	std::cin >> thermostat;
	while ((thermostat < mqttSettings::MINTEMP) || (thermostat > mqttSettings::MAXTEMP)) {
		std::cout << thermostat << " is an invalid temperature\n";
		std::cout << "Set the thermostat to a temperature, in Fahrenheit, within the range of " << 
		mqttSettings::MINTEMP << "-" << mqttSettings::MAXTEMP << ":\n";

		std::cin >> thermostat;
	}

	// This terminal will publish the temperature
	std::system("gnome-terminal -t temperature -- bash -c './temperature'");

	std::stringstream ss;
	ss << "gnome-terminal -t fanControl -- bash -c './fanControl -t " << thermostat << "'";
	// This terminal will control the fan
	std::system(ss.str().c_str());

	return 0;
}	
