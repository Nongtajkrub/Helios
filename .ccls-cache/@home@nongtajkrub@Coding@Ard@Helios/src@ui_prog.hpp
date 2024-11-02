#pragma once

#include "ui.hpp"

#include "button.hpp"

namespace program {
	typedef enum : u8 {
		MAIN,
		SETTING,
		MODE_SETTING,
		QUICK_ACT,
		TOGGLE_QUICKACT
	} ui_menu_t;

	struct ui_data {
		ui_menu_t on_menu;
		// whether the ui have to be update
		bool update;
		I2C* lcd;

		// use for communicating with light
		struct light_data* light;

		struct {
			ui::elem_t header_txt;
			ui::elem_t setting_opt;
			ui::elem_t quickact_opt;

			ui::group_t group;
		} main;

		struct {
			ui::elem_t header_txt;
			ui::elem_t mode_opt;
			ui::elem_t back_opt;

			ui::group_t group;
		} setting;

		struct {
			ui::elem_t header_txt;
			ui::elem_t auto_opt;
			ui::elem_t manu_opt;
			ui::elem_t back_opt;
			
			ui::group_t group;
		} setting_mode;

		struct {
			ui::elem_t header_txt;
			ui::elem_t toggle_opt;
			ui::elem_t back_opt;

			ui::group_t group;
		} quickact; 

		struct {
			ui::elem_t header_txt;
			ui::elem_t on_opt;
			ui::elem_t off_opt;
			ui::elem_t back_opt;

			ui::group_t group;
		} quickact_toggle;

		button::butt_t up_button;
		button::butt_t down_button;
		button::butt_t sel_button;
	};

	void ui_init(struct ui_data* ui, struct light_data* light);
	void ui_loop(struct ui_data* ui);
}
