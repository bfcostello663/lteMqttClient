# lteMqttClient

The following program will consist of 2 MQTT clients being:
	a. A temperature publisher
	b. A fan controller, subscribed to the temperature, that will turn the fan on or off

Dependencies:
-------------
	* cmake V3.7+
	* paho.mqtt.c v1.3.12 installed
	* paho.mqtt.cpp v1.2.0 installed
	* gnome-terminal

Development Environment:
------------------------
	* Ubuntu 22.04.2 LTS
	* Linux kernel version 5.19.0-38-generic
	* g++ (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0
	* cmake version 3.22.1
	* GNOME Terminal 3.44.0 using VTE 0.68.0 +BIDI +GNUTLS +ICU +SYSTEMD

Build Instructions:
-------------------
	* Execute the build.sh bash script

Usage:
------
	1. Execute run.sh which will prompt the user for a temperature between 60-90 degrees F for the thermostat.
	2. This will spawn two gnome shells named "fanControl" and "temperature".
	3. The temperature window will allow the user to enter a temperature within the valid range, or q to quit.
		- Creates the MQTT client "temperature-publish", which acts as a publisher
		- Temperature values will be published using the "cPlusPlus/client/lte/temperature" topic
		- q entries will be relayed over the same topic, and will stop this client
	4. The fanControl window will be started with the initial thermostat value, and turn the fan off/on based
	   off of the temperature
		- Creates the MQTT client "fan-control" that subscribes to the "cPlusPlus/client/lte/temperature"
		  topic
		- Temperature values will be consumed, and the resulting fan on/off status will be displayed
		- Any non integer value received will stop this MQTT client.

Sample I/O For The Thermostat setting
-------------------------------------
Set the thermostat to a temperature, in Fahrenheit, within the range of 60-90:
67

Sample I/O For The temperature Terminal:
----------------------------------------
temperature-publish: Connected to MQTT broker
Enter the living room temperature in Fahrenheit (60-90) or q to quit: 
67
Send '67' to topic 'cPlusPlus/client/lte/temperature'

Enter the living room temperature in Fahrenheit (60-90) or q to quit: 
61
Send '61' to topic 'cPlusPlus/client/lte/temperature'

Enter the living room temperature in Fahrenheit (60-90) or q to quit: 
90
Send '90' to topic 'cPlusPlus/client/lte/temperature'

Enter the living room temperature in Fahrenheit (60-90) or q to quit: 
91
91 is an invalid temperature
Enter the living room temperature in Fahrenheit (60-90) or q to quit: 
59
59 is an invalid temperature
Enter the living room temperature in Fahrenheit (60-90) or q to quit: 
67
Send '67' to topic 'cPlusPlus/client/lte/temperature'

Enter the living room temperature in Fahrenheit (60-90) or q to quit: 
67
Send '67' to topic 'cPlusPlus/client/lte/temperature'

Enter the living room temperature in Fahrenheit (60-90) or q to quit: 
q



Sample I/O For The fanControl Terminal:
---------------------------------------
fan-control: Connected to MQTT broker
fan-control: Subscribed to topic: cPlusPlus/client/lte/temperature
Current Temperature is 67F and thermostat set to 67F - fan on
Current Temperature is 61F and thermostat set to 67F - fan off
Current Temperature is 90F and thermostat set to 67F - fan on
Current Temperature is 67F and thermostat set to 67F - fan on

Noted Limitations:
------------------
	1. The fan-control MQTT client does not exit if the temperature-publish terminal window is killed.
	2. The thermostat value cannot be changed once it is set.

