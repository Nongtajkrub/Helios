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
		Serial.print(".");
		delay(1000);
	}

	Serial.println();
	Serial.println("Connected to Wifi");
}

struct program::data program_data;

void setup() {
	//connect_wifi();
	program::setup(&program_data);
}

void loop() {
	program::loop(&program_data);
}

/*
#include "type.hpp"

constexpr u8 LDR_PIN[] = {33, 32, 34, 35}; 

void setup() {
	Serial.begin(9600);

	for (const u8 pin : LDR_PIN) {
		pinMode(pin, INPUT);
	}
}

void loop() {
	for (const u8 pin : LDR_PIN) {
		Serial.print(pin);
		Serial.print(": ");
		Serial.println(analogRead(pin));
	}
	Serial.println();
	delay(500);
}

#include "type.hpp"

constexpr u8 BUTT_PIN[] = {12, 14, 26}; 

void setup() {
	Serial.begin(9600);

	for (const u8 pin : BUTT_PIN) {
		pinMode(pin, INPUT);
	}
}

void loop() {
	for (const u8 pin : BUTT_PIN) {
		Serial.print(pin);
		Serial.print(": ");
		Serial.println(digitalRead(pin));
	}
	Serial.println();
	delay(500);
}

#include "type.hpp"
#include "neopixel.hpp"

#define NP_COUNT 4
constexpr u8 NP_PIN[] = {19, 18, 17, 15}; 
np::pixel_t* pixel[NP_COUNT];

void setup() {
	Serial.begin(9600);

	for (u8 i = 0; i < NP_COUNT; i++) {
		pixel[i] = new Adafruit_NeoPixel(1, NP_PIN[i]);
		np::begin(pixel[i]);
		np::color(pixel[i], 255, 0, 0);
		np::brightness(pixel[i], 50);
		np::show(pixel[i]);
	}
}

void loop() {
	delay(500);
}
*/
