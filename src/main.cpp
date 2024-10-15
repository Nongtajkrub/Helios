#define LCD_ADDR 0x27
#define LCD_COLS 20
#define LCD_ROWS 4

#include "ui.hpp"
#include "electronics.hpp"
#include <Arduino.h>
#include <util/type.hpp>

// ui
I2C screen(LCD_ADDR, LCD_COLS, LCD_ROWS);

ui::elem_t* elem1;
ui::elem_t* elem2;
ui::elem_t* elem3;
ui::elem_t* elem4;
ui::group_t* group;

button::butt_t* butt1;
button::butt_t* butt2;
button::butt_t* butt3;

ldr::ldr_t* ldr1;

np::pixel_t* pixel;

void setup() {
	screen.init();
	elem1 = ui::make("Welcome", ui::TXT);
	elem2 = ui::make("Opt1", ui::OPT);
	elem3 = ui::make("Opt2", ui::OPT);
	elem4 = ui::make("Opt3", ui::OPT);
	group = ui::group(&screen, 4, elem1, elem2, elem3, elem4);

	butt1 = button::make(27);
	butt2 = button::make(12);
	butt3 = button::make(14);

	ldr1 = ldr::make(34);

	pixel = np::make(26, 1);

	Serial.begin(9600);
}

static void handle_select(struct ui::selector_in info) {
	switch (info.id) {
	case 0:
		Serial.println("Invalid");
		break;
	case 1:
		Serial.println(ldr::read(ldr1));
		break;
	case 2:
		Serial.println("On");
		np::color(pixel, 100, 0, 0);
		break;
	case 3:
		Serial.println("Off");
		np::color(pixel, 0, 0, 0);
		break;
	default:
		break;
	}
}

void loop() {
	ui::show(group);

	if (button::state(butt1)) {
		handle_select(ui::selector_on(group));
	} else if (button::state(butt2)) {
		ui::selector_up(group);
	} else if (button::state(butt3)) {
		ui::selector_down(group);
	}

	delay(50);
}
