/*
 * This UI system is design so bad I think a horse was shove up my ass 
 * while I was writing it. If this is still here I forgot to delete it.
*/

#include "ui_prog.hpp"
#include "light_prog.hpp"

#define GET_UI_SETTINGS 
#include "settings.hpp"

#define MENU_GROUP(MENU, COUNT, ...)                                          \
	do {                                                                      \
		ui::group(                                                            \
			&(MENU).group,                                                    \
			ui->lcd,                                                          \
			LCD_ROWS,                                                         \
			LCD_COLS,                                                         \
			(COUNT),                                                          \
			__VA_ARGS__                                                       \
			);                                                                \
	} while (0)

#define DEF_MENU_LOOP(MENU)                                                   \
	do {                                                                      \
		ui::show(&(MENU).group);                                              \
		control_loop(ui, &(MENU).group);                                      \
	} while (0) 

#define NOT_MANU_ERR_MSG "Enter manual mode first"

namespace program {
	static void init_main_menu(struct ui_data* ui) {
		ui::make(&ui->main.header_txt, "Welcome!", ui::TXT);
		ui::make(&ui->main.setting_opt, "Settings", ui::OPT);
		ui::make(&ui->main.quickact_opt, "QuickAct", ui::OPT);
		MENU_GROUP(
			ui->main,
			3,
			&ui->main.header_txt,
			&ui->main.setting_opt,
			&ui->main.quickact_opt
			);
	}

	static void init_setting_menu(struct ui_data* ui) {
		ui::make(&ui->setting.header_txt, "Settings", ui::TXT);
		ui::make(&ui->setting.mode_opt, "mode", ui::OPT);
		ui::make(&ui->setting.back_opt, BACK_UI_CHAR, ui::OPT);
		MENU_GROUP(
			ui->setting,
			3,
			&ui->setting.header_txt,
			&ui->setting.mode_opt,
			&ui->setting.back_opt
			);
	}

	static void init_setting_mode_menu(struct ui_data* ui) {
		ui::make(&ui->setting_mode.header_txt, "Mode", ui::TXT);
		ui::make(&ui->setting_mode.auto_opt, "auto", ui::OPT);
		ui::make(&ui->setting_mode.manu_opt, "manual", ui::OPT);
		ui::make(&ui->setting_mode.back_opt, BACK_UI_CHAR, ui::OPT);
		MENU_GROUP(
			ui->setting_mode,
			4,
			&ui->setting_mode.header_txt,
			&ui->setting_mode.auto_opt,
			&ui->setting_mode.manu_opt,
			&ui->setting_mode.back_opt
			);
	}

	static void init_quickact_menu(struct ui_data* ui) {
		ui::make(&ui->quickact.header_txt, "Quick Action", ui::TXT);
		ui::make(&ui->quickact.toggle_opt, "toggle", ui::OPT);
		ui::make(&ui->quickact.back_opt, BACK_UI_CHAR, ui::OPT);
		MENU_GROUP(
			ui->quickact,
			3,
			&ui->quickact.header_txt,
			&ui->quickact.toggle_opt,
			&ui->quickact.back_opt
			);
	}

	static void init_quickact_toggle_menu(struct ui_data* ui) {
		ui::make(&ui->quickact_toggle.header_txt, "Toggle", ui::TXT);
		ui::make(&ui->quickact_toggle.on_opt, "on", ui::OPT);
		ui::make(&ui->quickact_toggle.off_opt, "off", ui::OPT);
		ui::make(&ui->quickact_toggle.back_opt, BACK_UI_CHAR, ui::OPT);
		MENU_GROUP(
			ui->quickact_toggle,
			4,
			&ui->quickact_toggle.header_txt, 
			&ui->quickact_toggle.on_opt, 
			&ui->quickact_toggle.off_opt,
			&ui->quickact_toggle.back_opt
			);
	}

	static void init_button(struct ui_data* ui) {
		button::make(&ui->up_button, UP_BUTT_PIN);
		button::make(&ui->down_button, DOWN_BUTT_PIN);
		button::make(&ui->sel_button, SEL_BUTT_PIN);
	}

	void ui_init(struct ui_data* ui, struct light_data* light) {
		ui->on_menu = MAIN;

		// init lcd
		ui->lcd = new I2C(LCD_ADDR, LCD_COLS, LCD_ROWS);
		ui->lcd->init();

		ui->light = light;

		init_main_menu(ui);
		init_setting_menu(ui);
		init_setting_mode_menu(ui);
		init_quickact_menu(ui);
		init_quickact_toggle_menu(ui);

		init_button(ui);
	}

	static void control_main_handle_sel(
		struct ui_data* ui,
		ui::group_t* group
		) {
		switch (ui::selector_on(group).id) {
		case 0: // header_txt
			break;
		case 1: // setting_opt
			ui->on_menu = SETTING;
			break;
		case 2: // quickact_opt:
			ui->on_menu = QUICK_ACT;	
			break;
		}
	}

	static void control_setting_handle_sel(
		struct ui_data* ui,
		ui::group_t* group
		) {
		switch (ui::selector_on(group).id) {
		case 0: // header_txt
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
		case 0: // header_txt
			break;
		case 1: // auto_opt
			light_set_mode_manu(ui->light, false);
			break;
		case 2: // manu_opt
			light_set_mode_manu(ui->light, true);
			break;
		case 3: // back_opt
			ui->on_menu = SETTING;
			break;
		}
	}

	static void control_quickact_handle_sel(
		struct ui_data* ui,
		ui::group_t* group
		) {
		switch (ui::selector_on(group).id) {
		case 0: // header_txt
			break;
		case 1: // toggle
			ui->on_menu = TOGGLE_QUICK_ACT;
			break;
		case 2: // back_opt
			ui->on_menu = MAIN;
			break;
		}
	}

	static void control_quickact_toggle_handle_sel(
		struct ui_data* ui,
		ui::group_t* group
		) {
		switch (ui::selector_on(group).id) {
		case 0: // header_txt
			break;
		case 1: // on_opt
			light_set_on(ui->light, true);
			break;
		case 2: // off_opt
			light_set_on(ui->light, false);
			break;
		case 3: // back_opt
			ui->on_menu = QUICK_ACT;
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
		case QUICK_ACT:
			control_quickact_handle_sel(ui, group);
			break;
		case TOGGLE_QUICK_ACT:
			control_quickact_toggle_handle_sel(ui, group);
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

	// quickact toggle menu only work in manual mode
	static void qucikact_menu_loop(struct ui_data* ui) {
		// show error and go back to main menu if not in manual mode
		if (!light_is_manu(ui->light)) {
			ui::show(NOT_MANU_ERR_MSG, ui->lcd, 0, 0, LCD_COLS);

			// delay 3 seconds
			vTaskDelay(3000 / portTICK_PERIOD_MS);
			ui->on_menu = MAIN;
			return;
		}

		// show menu if in manual mode
		ui::show(&ui->quickact.group);
		control_loop(ui, &ui->quickact.group);
	}

	void ui_loop(struct ui_data* ui) {
		switch (ui->on_menu) {
		case MAIN:
			DEF_MENU_LOOP(ui->main);
			break;
		case SETTING:
			DEF_MENU_LOOP(ui->setting);
			break;
		case MODE_SETTING:
			DEF_MENU_LOOP(ui->setting_mode);
			break;
		case QUICK_ACT:
			qucikact_menu_loop(ui);
			break;
		case TOGGLE_QUICK_ACT:
			DEF_MENU_LOOP(ui->quickact_toggle);
			break;
		}
	}
}
