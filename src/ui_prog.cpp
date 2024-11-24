#include "ui_prog.hpp"
#include "light_prog.hpp"

#define GET_UI_SETTINGS
#include "settings.hpp"

#define OPT_MAKE(MENU, HEADER, NUM, ...)                                      \
	do {                                                                      \
		ui::opt_make(MENU, HEADER, &screen, &ui->selc, NUM, __VA_ARGS__);     \
	} while (0) 

#define MAKE_CH_MENU_TRIG(NAME, MENU_ENUM, MENU_OBJ)                          \
	static inline void (NAME)(void* arg) {                                    \
		struct ui_data* ui = (struct ui_data*)arg;                            \
                                                                              \
		ui->on_menu = MENU_ENUM;                                              \
		ui::reset_selc(&ui->selc);                                            \
	}                                                                         

#define DEF_MENU_LOOP(MENU)                                                   \
	do {                                                                      \
		if (ui->e != ui::NONE) {                                              \
			ui::opt_show(MENU.opt);                                           \
			ui::event_take(&ui->e);                                           \
		}                                                                     \
		                                                                      \
		ui::opt_loop(MENU.opt);                                               \
	} while (0) 

namespace program {
	static I2C screen(LCD_ADDR, LCD_COLS, LCD_ROWS);

	MAKE_CH_MENU_TRIG(handle_setting_selc, SETTING, &ui->setting_menu);

	static void handle_quickact_selc(void* arg) {
		struct ui_data* ui = (struct ui_data*)arg;

		if (!light_is_manu(ui->light)) {
			screen.clear();
			ui::show(&screen, NOT_MANU_ERR_MSG, 0, 0, LCD_COLS, LCD_COLS);
			vTaskDelay(3000 / portTICK_PERIOD_MS);
			return;
		}

		ui->on_menu = QUICKACT;
		ui::reset_selc(&ui->selc);
	}

	static void init_main_menu(struct ui_data* ui) {
		struct ui_main_menu* menu = &ui->main_menu;

		ui::opt_elem_make(&menu->setting, "Settings", handle_setting_selc, ui);
		ui::opt_elem_make(&menu->quickact, "Quickact", handle_quickact_selc, ui);

		OPT_MAKE(&menu->opt, "Main", &ui->e, 2, &menu->setting, &menu->quickact);
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

		ui::reset_selc(&ui->selc);
	}

	MAKE_CH_MENU_TRIG(handle_mode_selc, MODE, &ui->mode_menu);

	static void init_setting_menu(struct ui_data* ui) {
		struct ui_setting_menu* menu = &ui->setting_menu;

		ui::opt_elem_make(&menu->mode, "Mode", handle_mode_selc, ui);
		ui::opt_elem_make(&menu->back, BACK_INDICATOR, handle_back_selc, ui);

		OPT_MAKE(&menu->opt, "Settings", &ui->e, 2, &menu->mode, &menu->back);
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

		OPT_MAKE(&menu->opt, "Mode", &ui->e, 3, &menu->_auto, &menu->manual, &menu->back);
	}

	MAKE_CH_MENU_TRIG(handle_toggle_selc, TOGGLE, &ui->toggle_menu);

	static void init_quickact_menu(struct ui_data* ui) {
		struct ui_quickact_menu* menu = &ui->quickact_menu;

		ui::opt_elem_make(&menu->toggle, "Toggle", handle_toggle_selc, ui);
		ui::opt_elem_make(&menu->back, BACK_INDICATOR, handle_back_selc, ui);

		OPT_MAKE(&menu->opt, "Quickact", &ui->e, 2, &menu->toggle, &menu->back);
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

		OPT_MAKE(&menu->opt, "Toggle", &ui->e, 3, &menu->on, &menu->off, &menu->back);
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
		init_quickact_menu(ui);
		init_toggle_menu(ui);
		ui::opt_show(&ui->main_menu.opt);

		init_control(ui);

		ui->light = light;
	}

	void ui_loop(struct ui_data* ui) {
		switch (ui->on_menu) {
		case MAIN:
			DEF_MENU_LOOP(&ui->main_menu);
			break;
		case SETTING:
			DEF_MENU_LOOP(&ui->setting_menu);
			break;
		case MODE:
			DEF_MENU_LOOP(&ui->mode_menu);
			break;
		case QUICKACT:
			DEF_MENU_LOOP(&ui->quickact_menu);
			break;
		case TOGGLE:
			DEF_MENU_LOOP(&ui->toggle_menu);
			break;
		default:
			break;
		}
	}
}
