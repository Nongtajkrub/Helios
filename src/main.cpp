#include "program.hpp"
#include "type.h"
#include <Arduino.h>
#include <WiFi.h>

// hidden.hpp is hide by gitignore to create one check the example in README
#define GET_WIFI_HIDDEN
#include "hidden.hpp"

static void connect_wifi() {
	WiFi.begin(WIFI_SSID, WIFI_PASS);
	u8 attempt = 0;
	
	while (WiFi.status() != WL_CONNECTED) {
		if (attempt >= 10) {
			return;
		}

		attempt++;
		delay(1000);
	}
}

struct program::data program_data;

void setup() {
	Serial.begin(9600);

	connect_wifi();
	program::setup(&program_data);
}

void loop() {
	// dont need to do anything vTask handle the loop
}
