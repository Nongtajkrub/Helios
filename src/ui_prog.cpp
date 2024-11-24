#include "ui_prog.hpp"
#include "light_prog.hpp"

#define GET_UI_SETTINGS
#include "settings.hpp"

#define OPT_MAKE(MENU, HEADER, NUM, ...)                                      \
	do {                                                                      \
		ui::opt_make(MENU, HEADER, &screen, &ui->selc, NUM, __VA_ARGS__);     \
	} while (0) 

#define MAKE_CH_MENU_TRIG(NAME, MENU)                                         \
	static inline void (NAME)(void* arg) {                                    \
		struct ui_data* ui = (struct ui_data*)arg;                            \
                                                                              \
		ui->on_menu = MENU;                                                   \
	}                                                                         \

#define BACK_INDICATOR "<"

namespace program {
	static I2C screen(LCD_ADDR, LCD_COLS, LCD_ROWS);

	MAKE_CH_MENU_TRIG(handle_setting_selc, SETTING);
	MAKE_CH_MENU_TRIG(handle_quickact_selc, QUICKACT);

	static void init_main_menu(struct ui_data* ui) {
		struct ui_main_menu* menu = &ui->main_menu;

		ui::opt_elem_make(&menu->setting, "Settings", handle_setting_selc, ui);
		ui::opt_elem_make(&menu->quickact, "Quickact", handle_quickact_selc, ui);

		OPT_MAKE(&menu->opt, "Main", 2, &menu->setting, &menu->quickact);
	}

	static void handle_back_selc(void* arg) {
		struct ui_data* ui = (struct ui_data*)arg;

		switch (ui->on_menu) {
		case SETTING:
			ui->on_menu = MAIN;
			break;
		case MODE:
			ui->on_menu = SETTING;
			break;
		case QUICKACT:
			ui->on_menu = MAIN;
			break;
		case TOGGLE:
			ui->on_menu = QUICKACT;
			break;
		default:
			break;
		}
	}

	MAKE_CH_MENU_TRIG(handle_mode_selc, MODE);
	MAKE_CH_MENU_TRIG(handle_setting_back_selc, MAIN);

	static void init_setting_menu(struct ui_data* ui) {
		struct ui_setting_menu* menu = &ui->setting_menu;

		ui::opt_elem_make(&menu->mode, "Mode", handle_mode_selc, ui);
		ui::opt_elem_make(&menu->back, BACK_INDICATOR, handle_back_selc, ui);

		OPT_MAKE(&menu->opt, "Settings", 2, &menu->mode, &menu->back);
	}

	static inline void handle_auto_selc(void* arg) {
		struct ui_data* ui = (struct ui_data*)arg;

		light_set_mode_manu(ui->light, false);
	}

	static inline void handle_manual_selc(void* arg) {
		struct ui_data* ui = (struct ui_data*)arg;

		light_set_mode_manu(ui->light, true);
	}

	static void init_mode_menu(struct ui_data* ui) {
		struct ui_mode_menu* menu = &ui->mode_menu;

		ui::opt_elem_make(&menu->_auto, "Auto", handle_auto_selc, ui);
		ui::opt_elem_make(&menu->manual, "Manual", handle_manual_selc, ui);
		ui::opt_elem_make(&menu->back, BACK_INDICATOR, handle_back_selc, ui);

		OPT_MAKE(&menu->opt, "Mode", 3, &menu->_auto, &menu->manual, &menu->back);
	}

	MAKE_CH_MENU_TRIG(handle_toggle_selc, TOGGLE);

	static void init_quickact_menu(struct ui_data* ui) {
		struct ui_quickact_menu* menu = &ui->quickact_menu;

		ui::opt_elem_make(&menu->toggle, "Toggle", handle_toggle_selc, ui);
		ui::opt_elem_make(&menu->back, BACK_INDICATOR, handle_back_selc, ui);

		OPT_MAKE(&menu->opt, "Quickact", 2, &menu->toggle, &menu->back);
	}

	static inline void handle_on_selc(void* arg) {
		struct ui_data* ui = (struct ui_data*)arg;

		light_set_on(ui->light, true);
	}

	static inline void handle_off_selc(void* arg) {
		struct ui_data* ui = (struct ui_data*)arg;

		light_set_on(ui->light, false);
	}

	static void init_toggle_menu(struct ui_data* ui) {
		struct ui_toggle_menu* menu = &ui->toggle_menu;

		ui::opt_elem_make(&menu->on, "On", handle_on_selc, ui);
		ui::opt_elem_make(&menu->off, "off", handle_off_selc, ui);
		ui::opt_elem_make(&menu->back, BACK_INDICATOR, handle_back_selc, ui);

		OPT_MAKE(&menu->opt, "Toggle", 3, &menu->on, &menu->off, &menu->back);
	}

	static bool up_trig(void* arg) {
		struct ui_data* ui = (struct ui_data*)arg;

		return button::state(&ui->up_button);
	}

	static bool down_trig(void* arg) {
		struct ui_data* ui = (struct ui_data*)arg;

		return button::state(&ui->down_button);
	}

	static bool selc_trig(void* arg) {
		struct ui_data* ui = (struct ui_data*)arg;

		return button::state(&ui->selc_button);
	}

	static void init_control(struct ui_data* ui) {
		button::make(&ui->up_button, UP_BUTT_PIN);
		button::make(&ui->down_button, DOWN_BUTT_PIN);
		button::make(&ui->selc_button, SELC_BUTT_PIN);
		ui::selector_make(&ui->selc, up_trig, down_trig, selc_trig, ui);
	}

	void ui_init(struct ui_data* ui, struct light_data* light) {
		ui->on_menu = MAIN;
		ui->update = false;

		ui::ready_screen(&screen);
		init_main_menu(ui);
		init_setting_menu(ui);
		init_mode_menu(ui);

		init_control(ui);

		ui->light = light;
	}

	void ui_loop(struct ui_data* ui) {
		switch (ui->on_menu) {
		case MAIN:
			ui::opt_show(&ui->main_menu.opt);
			ui::opt_loop(&ui->main_menu.opt);
			break;
		case SETTING:
			ui::opt_show(&ui->setting_menu.opt);
			ui::opt_loop(&ui->setting_menu.opt);
			break;
		case MODE:
			ui::opt_show(&ui->mode_menu.opt);
			ui::opt_loop(&ui->mode_menu.opt);
			break;
		case QUICKACT:
			ui::opt_show(&ui->quickact_menu.opt);
			ui::opt_loop(&ui->quickact_menu.opt);
			break;
		case TOGGLE:
			ui::opt_show(&ui->toggle_menu.opt);
			ui::opt_loop(&ui->toggle_menu.opt);
			break;
		default:
			break;
		}
	}
}
