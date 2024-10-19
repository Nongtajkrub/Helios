#include "ui_prog.hpp"

#define GET_UI_SETTINGS 
#include "settings.hpp"

#define MENU_GROUP(MENU, COUNT, ...) \
	do {                             \
		ui::group(                   \
			&(MENU).group,           \
			ui->lcd,                 \
			LCD_ROWS,                \
			LCD_COLS,                \
			(COUNT),                 \
			__VA_ARGS__              \
			);                       \
	} while (0)

#define MENU_LOOP(MENU)                  \
	do {                                 \
		menu_loop(&(MENU).group);        \
		control_loop(ui, &(MENU).group); \
	} while (0) 

namespace program {
	static void init_main_menu(struct ui_data* ui) {
		ui::make(&ui->main.welcom_txt, "Welcome!", ui::TXT);
		ui::make(&ui->main.setting_opt, "Settings", ui::OPT);
		MENU_GROUP(
			ui->main,
			2,
			&ui->main.welcom_txt,
			&ui->main.setting_opt
			);
	}

	static void init_setting_menu(struct ui_data* ui) {
		ui::make(&ui->setting.setting_txt, "Settings", ui::TXT);
		ui::make(&ui->setting.mode_opt, "mode", ui::OPT);
		ui::make(&ui->setting.back_opt, BACK_UI_CHAR, ui::OPT);
		MENU_GROUP(
			ui->setting,
			3,
			&ui->setting.setting_txt,
			&ui->setting.mode_opt,
			&ui->setting.back_opt
			);
	}

	static void init_setting_mode_menu(struct ui_data* ui) {
		ui::make(&ui->setting_mode.mode_txt, "Mode", ui::TXT);
		ui::make(&ui->setting_mode.auto_opt, "auto", ui::OPT);
		ui::make(&ui->setting_mode.manu_opt, "manual", ui::OPT);
		ui::make(&ui->setting_mode.back_opt, BACK_UI_CHAR, ui::OPT);
		MENU_GROUP(
			ui->setting_mode,
			4,
			&ui->setting_mode.mode_txt,
			&ui->setting_mode.auto_opt,
			&ui->setting_mode.manu_opt,
			&ui->setting_mode.back_opt
			);
	}

	static void init_button(struct ui_data* ui) {
		button::make(&ui->up_button, UP_BUTT_PIN);
		button::make(&ui->down_button, DOWN_BUTT_PIN);
		button::make(&ui->sel_button, SEL_BUTT_PIN);
	}

	void ui_init(struct ui_data* ui) {
		ui->on_menu = MAIN;
		stack_make(&ui->req, sizeof(ui_request_t));

		// init lcd
		ui->lcd = new I2C(LCD_ADDR, LCD_COLS, LCD_ROWS);
		ui->lcd->init();

		init_main_menu(ui);
		init_setting_menu(ui);
		init_setting_mode_menu(ui);

		init_button(ui);
	}

	static inline void menu_loop(ui::group_t* group) {
		ui::show(group);
	}

	static inline void ui_request(struct ui_data* ui, ui_request_t req) {
		stack_push(&ui->req, (void*)&req);
	}

	static void control_main_handle_sel(
		struct ui_data* ui,
		ui::group_t* group
		) {
		switch (ui::selector_on(group).id) {
		case 0: // welcom_txt
			break;
		case 1: // setting_opt
			ui->on_menu = SETTING;
			break;
		}
	}

	static void control_setting_handle_sel(
		struct ui_data* ui,
		ui::group_t* group
		) {
		switch (ui::selector_on(group).id) {
		case 0: // setting_txt
			break;
		case 1: // mode_opt
			ui->on_menu = MODE_SETTING;
			break;
		case 2: // back_opt
			ui->on_menu = MAIN;
			break;
		}
	}

	static void control_setting_mode_handle_sel(
		struct ui_data* ui,
		ui::group_t* group
		) {
		switch (ui::selector_on(group).id) {
		case 0: // mode_txt
			break;
		case 1: // auto_opt
			ui_request(ui, SETTING_MODE_AUTO);
			break;
		case 2: // manu_opt
			ui_request(ui, SETTING_MODE_MANU);
			break;
		case 3: // back_opt
			ui->on_menu = SETTING;
			break;
		}
	}
	
	static void control_handle_sel(struct ui_data* ui, ui::group_t* group) {
		switch (ui->on_menu) {
		case MAIN:
			control_main_handle_sel(ui, group);
			break;
		case SETTING:
			control_setting_handle_sel(ui, group);
			break;
		case MODE_SETTING:
			control_setting_mode_handle_sel(ui, group);
			break;
		}
	}

	static void control_loop(struct ui_data* ui, ui::group_t* group) {
		if (button::state(&ui->up_button)) {
			ui::selector_up(group);
		} else if (button::state(&ui->down_button)) {
			ui::selector_down(group);
		} else if (button::state(&ui->sel_button)) {
			control_handle_sel(ui, group);
		}
	}

	void ui_loop(struct ui_data* ui) {
		switch (ui->on_menu) {
		case MAIN:
			MENU_LOOP(ui->main);
			break;
		case SETTING:
			MENU_LOOP(ui->setting);
			break;
		case MODE_SETTING:
			MENU_LOOP(ui->setting_mode);
			break;
		}
	}
}
