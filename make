#!/bin/bash

if arduino-cli compile -t --fqbn arduino:avr:mega pacman; then
	arduino-cli upload -v -p /dev/ttyACM0 --fqbn arduino:avr:mega pacman
	sleep 1
	picocom -b 9600 /dev/ttyACM0
else 
	echo "Build failed"
fi
