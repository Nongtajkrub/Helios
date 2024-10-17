#pragma once

#include "ui.hpp"
#include "button.hpp"

namespace program {
	typedef enum {
		MAIN,
		SETTING
	} ui_menu_t;

	struct ui_data {
		ui_menu_t on_menu;
		I2C* lcd;

		// main menu element
		struct {
			ui::elem_t* welcom_txt;
			ui::elem_t* setting_opt;
			ui::elem_t* control_opt;
			
			ui::group_t* group;
		} main;

		// setting menu element
		struct {
			ui::elem_t* setting_txt;
			ui::elem_t* mode_opt;
			
			ui::group_t* group;
		} setting;

		button::butt_t* up_button;
		button::butt_t* down_button;
		button::butt_t* sel_button;
	};

	void init_ui(struct ui_data* ui);
	void loop_ui(struct ui_data* ui);
}
