
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sstream>


int main() {
	int thermastat = 70;

	std::cout << "Set the thermastat to a temparature, in Farenheit, within the range of 60-90:\n";
	std::cin >> thermastat;
	while ((thermastat < 60) || (thermastat > 90)) {
		std::cout << thermastat << " is an invalid teperature\n";
		std::cout << "Set the thermastat to a temparature, in Farenheit, within the range of 60-90:\n";
		std::cin >> thermastat;
	}

	// This terminal will publish the temparature
	std::system("gnome-terminal -t temparature -- bash -c './temparature'");

	std::stringstream ss;
	ss << "gnome-terminal -t fanControl -- bash -c './fanControl -t " << thermastat << "'";
	// This terminal will control the fan
	std::system(ss.str().c_str());

	return 0;
}	
