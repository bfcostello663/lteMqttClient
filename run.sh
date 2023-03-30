#!/bin/bash

[ ! -d "build" ] && echo "Run build.sh to generate the binary, and then retry" && exit -1
cd build
./lteMQTTClient

