#pragma once

#include "ui.hpp"
#include "electronics.hpp"

namespace program {
	typedef enum {
		MAIN,
		SETTING
	} ui_menu_t;

	struct ui_data {
		ui_menu_t on_menu;
		I2C* lcd;

		struct {
			ui::elem_t* welcom_txt;
			ui::elem_t* setting_opt;
			ui::elem_t* control_opt;
			ui::group_t* group;
		} main;
		
		button::butt_t* up_button;
		button::butt_t* down_button;
		button::butt_t* sel_button;
	};

	struct ldr_data {
		ldr::ldr_t* ldr1;
	};
	
	struct np_data {
		np::pixel_t* pixel;
	};

	struct data {
		struct ui_data ui;
		struct ldr_data ldr;
		struct np_data np;
	};

	void setup(struct data* data);
	void loop(struct data* data);
}
