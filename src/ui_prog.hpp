#pragma once

#include "ui.hpp"

#include "button.hpp"

namespace program {
typedef enum : u8 {
	MAIN,
	SETTING,
	MODE,
	COLOR,
	COLOR_R,
	COLOR_G,
	COLOR_B,
	QUICKACT,
	TOGGLE
} ui_menu_t;

struct ui_main_menu {
	ui::opt_elem_t setting;
	ui::opt_elem_t quickact;

	ui::opt_t opt;
};

struct ui_setting_menu {
	ui::opt_elem_t mode;
	ui::opt_elem_t color;

	ui::opt_t opt;
};

struct ui_mode_menu {
	// _auto to avoid conflict with keyword auto
	ui::opt_elem_t _auto;
	ui::opt_elem_t manual;

	ui::opt_t opt;
};

struct ui_quickact_menu {
	ui::opt_elem_t toggle;

	ui::opt_t opt;
};

struct ui_toggle_menu {
	ui::opt_elem_t on;
	ui::opt_elem_t off;

	ui::opt_t opt;
};

struct ui_color_menu {
	ui::opt_elem_t r;
	ui::opt_elem_t g;
	ui::opt_elem_t b;

	ui::num_t color_val;

	ui::opt_t opt;
};

struct ui_data {
	ui_menu_t on_menu;
	// whether the ui have to be update
	bool update;

	button::butt_t up_button;
	button::butt_t down_button;
	button::butt_t selc_button;
	ui::selector_t selc;

	ui::opt_elem_t back_butt;
	ui::event_t e;

	// menus
	struct ui_main_menu main_menu;

	struct ui_setting_menu setting_menu;
	struct ui_mode_menu mode_menu;
	struct ui_color_menu color_menu;

	struct ui_quickact_menu quickact_menu;
	struct ui_toggle_menu toggle_menu;

	// use for communicating with light
	struct light_data* light;
};

void ui_init(struct ui_data* ui, struct light_data* light);
void ui_loop(struct ui_data* ui);
}
