#include "program.hpp"
#include <Arduino.h>
#include "type.hpp"

struct program::data program_data;

void setup() {
	Serial.begin(9600);
	program::setup(&program_data);
}

void loop() {
	program::loop(&program_data);
}
