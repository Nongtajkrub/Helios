#pragma once

#include "ui.hpp"
#include "button.hpp"
#include "stack.h"

namespace program {
	typedef enum {
		MAIN,
		SETTING,
		MODE_SETTING,
		QUICK_ACT,
		TOGGLE_QUICK_ACT
	} ui_menu_t;

	// all requests ui can send to light controller
	typedef enum {
		SETTING_MODE_MANU,
		SETTING_MODE_AUTO,
		IS_MANU_ON
	} ui_req_t;

	// all respond ui can recv from light controller
	typedef enum {
		MANU_ON,
		MANU_OFF
	} ui_res_t;

	struct ui_data {
		ui_menu_t on_menu;
		I2C* lcd;

		// a stack containing UI requests to the light controller
		cstack_t req;
		// a stack containing response send to UI from light controller
		cstack_t res;

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

			ui::elem_t notmanu_txt;
			ui::elem_t error_txt;

			ui::group_t group;
		} quickact_toggle;

		button::butt_t up_button;
		button::butt_t down_button;
		button::butt_t sel_button;
	};

	void ui_init(struct ui_data* ui);
	void ui_loop(struct ui_data* ui);
}
