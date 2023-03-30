cmake_minimum_required(VERSION 3.7)
project(lteMQTTClient_proj)

add_executable(temparature temparature.cpp)
target_link_libraries(temparature paho-mqttpp3 paho-mqtt3as)

add_executable(fanControl fanControl.cpp)
target_link_libraries(fanControl paho-mqttpp3 paho-mqtt3as)

add_executable(lteMQTTClient main.cpp)

target_link_libraries(lteMQTTClient paho-mqttpp3 paho-mqtt3as)
