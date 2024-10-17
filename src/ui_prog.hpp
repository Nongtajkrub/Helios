#pragma once

#include "ui.hpp"
#include "button.hpp"

namespace program {
	typedef enum {
		MAIN,
		SETTING,
		MODE_SETTING
	} ui_menu_t;

	typedef enum {
		SETTING_MODE_MANU,
		SETTING_MODE_ATUO
	} ui_request_t;

	struct ui_data {
		ui_menu_t on_menu;
		ui_request_t req;
		I2C* lcd;

		// main menu element
		struct {
			ui::elem_t welcom_txt;
			ui::elem_t setting_opt;
			ui::elem_t control_opt;
			
			ui::group_t group;
		} main;

		// setting menu element
		struct {
			ui::elem_t setting_txt;
			ui::elem_t mode_opt;
			
			ui::group_t group;
		} setting;

		button::butt_t up_button;
		button::butt_t down_button;
		button::butt_t sel_button;
	};

	void ui_init(struct ui_data* ui);
	void ui_loop(struct ui_data* ui);
}
