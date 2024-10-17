#define LCD_ADDR 0x27
#define LCD_COLS 20
#define LCD_ROWS 4

#define UP_BUTT_PIN 12
#define DOWN_BUTT_PIN 14
#define SEL_BUTT_PIN 27

#include "ui_prog.hpp"

namespace program {
	static void init_main_menu(struct ui_data* ui) {
		ui->main.welcom_txt = ui::make("Welcome!", ui::TXT);
		ui->main.setting_opt = ui::make("Settings", ui::OPT);
		ui->main.control_opt = ui::make("Controls", ui::OPT);
		ui->main.group = ui::group(
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
		ui->setting.setting_txt = ui::make("Settings", ui::TXT);
		ui->setting.mode_opt = ui::make("mode", ui::OPT);
		ui->setting.group = ui::group(
				ui->lcd,
				LCD_ROWS,
				LCD_COLS,
				2,
				ui->setting.setting_txt,
				ui->setting.mode_opt
				);
	}

	void init_ui(struct ui_data* ui) {
		ui->on_menu = MAIN;

		// init lcd
		ui->lcd = new I2C(LCD_ADDR, LCD_COLS, LCD_ROWS);
		ui->lcd->init();

		// init menus
		init_main_menu(ui);
		init_setting_menu(ui);

		// init buttons
		ui->up_button = button::make(UP_BUTT_PIN);
		ui->down_button = button::make(DOWN_BUTT_PIN);
		ui->sel_button = button::make(SEL_BUTT_PIN);
	}

	/*
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

	static void control_handle_sel(struct ui_data* ui) {
		switch (ui->on_menu) {
		case MAIN:
			break;
		case SETTING:
			// TODO: handle settings menu
			break;
		}
	}

	static void control_loop(struct ui_data* ui, ui::group_t* group) {
		if (button::state(ui->up_button)) {
			ui::selector_up(group);
		} else if (button::state(ui->down_button)) {
			ui::selector_down(group);
		} else if (button::state(ui->sel_button)) {
			//TODO: handle select
		}
	}

	void loop_ui(struct ui_data* ui) {
		switch (ui->on_menu) {
		case MAIN:
			menu_loop(ui->main.group);
			break;
		case SETTING:
			// TODO: handle settings menu
			break;
		}
	}
	*/
}
