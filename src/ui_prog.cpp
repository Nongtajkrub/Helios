#include "ui_prog.hpp"

#define LCD_ADDR 0x27
#define LCD_COLS 20
#define LCD_ROWS 4

#define UP_BUTT_PIN 12
#define DOWN_BUTT_PIN 14
#define SEL_BUTT_PIN 27

#define LOOP(MENU)                      \
	do {                                \
		menu_loop(&(MENU).group);        \
		control_loop(ui, &(MENU).group); \
	} while (0) 

namespace program {
	static void init_main_menu(struct ui_data* ui) {
		ui::make(&ui->main.welcom_txt, "Welcome!", ui::TXT);
		ui::make(&ui->main.setting_opt, "Settings", ui::OPT);
		ui::make(&ui->main.control_opt, "Controls", ui::OPT);
		ui::group(
			&ui->main.group,
			ui->lcd,
			LCD_ROWS,
			LCD_COLS,
			3,
			ui->main.welcom_txt,
			ui->main.setting_opt,
			ui->main.control_opt
			);
	}

	static void init_setting_menu(struct ui_data* ui) {
		ui::make(&ui->setting.setting_txt, "Settings", ui::TXT);
		ui::make(&ui->setting.mode_opt, "mode", ui::OPT);
		ui::group(
			&ui->setting.group, 
			ui->lcd,
			LCD_ROWS,
			LCD_COLS,
			2,
			ui->setting.setting_txt,
			ui->setting.mode_opt
			);
	}

	void ui_init(struct ui_data* ui) {
		ui->on_menu = MAIN;

		// init lcd
		ui->lcd = new I2C(LCD_ADDR, LCD_COLS, LCD_ROWS);
		ui->lcd->init();

		// init menus
		init_main_menu(ui);
		init_setting_menu(ui);

		// init buttons
		button::make(&ui->up_button, UP_BUTT_PIN);
		button::make(&ui->down_button, DOWN_BUTT_PIN);
		button::make(&ui->sel_button, SEL_BUTT_PIN);
	}

	static void menu_loop(ui::group_t* group) {
		ui::show(group);
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
		case 2: // control_opt
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
			// TODO: handle setting mode menu
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
			LOOP(ui->main);
			break;
		case SETTING:
			LOOP(ui->setting);
			break;
		case MODE_SETTING:
			// TODO: handle mode setting
			break;
		}
	}
}
