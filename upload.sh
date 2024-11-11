if [ "$1" != "nc" ]; then
	sh compile.sh
fi

mv src/main.cpp src/src.ino
arduino-cli upload -p /dev/ttyUSB0 --fqbn esp32:esp32:esp32 src
mv src/src.ino src/main.cpp
