/*
#include "program.hpp"
#include "type.hpp"
#include <Arduino.h>
#include <WiFi.h>

// hidden.hpp is hide by gitignore to create one check the example in README
#define GET_WIFI_HIDDEN
#include "hidden.hpp"

static void connect_wifi() {
	Serial.println("Connecting to Wifi");
	WiFi.begin(WIFI_SSID, WIFI_PASS);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println();
	Serial.println("Connected to Wifi");
}

struct program::data program_data;

void setup() {
	Serial.begin(9600);

	connect_wifi();
	program::setup(&program_data);
}

void loop() {
	program::loop(&program_data);
}
*/

#define LDR_PIN 34

#include <Arduino.h>

void setup() {
	Serial.begin(9600);
	pinMode(LDR_PIN, INPUT);
}

void loop() {
	Serial.println(analogRead(LDR_PIN));
	delay(500);
}
