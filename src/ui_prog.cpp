#include "ui_prog.hpp"
#include "light_prog.hpp"

#define GET_UI_SETTINGS
#include "settings.hpp"

#define OPT_MAKE(HEADER, NUM, ...)                                            \
		ui::opt_make(                                                         \
			&menu->opt,                                                       \
			HEADER,                                                           \
			&screen,                                                          \
			LCD_ROWS,                                                         \
			&ui->selc,                                                        \
			&ui->e,                                                           \
			NUM,                                                              \
			__VA_ARGS__)     

#define MAKE_CH_MENU_TRIG(NAME, MENU_ENUM)                                    \
	static inline void (NAME)(void* arg) {                                    \
		struct ui_data* ui = (struct ui_data*)arg;                            \
                                                                              \
		ui->on_menu = MENU_ENUM;                                              \
		ui::selc_reset(&ui->selc);                                            \
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

MAKE_CH_MENU_TRIG(handle_setting_selc, SETTING);

static void handle_quickact_selc(void* arg) {
	struct ui_data* ui = (struct ui_data*)arg;

	if (!light_is_manu(ui->light)) {
		screen.clear();
		ui::show(&screen, NOT_MANU_ERR_MSG, 0, 0, LCD_COLS, LCD_COLS);
		vTaskDelay(3000 / portTICK_PERIOD_MS);
		return;
	}

	ui->on_menu = QUICKACT;
	ui::selc_reset(&ui->selc);
}

static void init_main_menu(struct ui_data* ui) {
	struct ui_main_menu* menu = &ui->main_menu;

	ui::opt_elem_make(&menu->setting, "Settings", handle_setting_selc, ui);
	ui::opt_elem_make(&menu->quickact, "Quickact", handle_quickact_selc, ui);

	OPT_MAKE("Main", 2, &menu->setting, &menu->quickact);
}

static void back_to_prev_menu(void* arg) {
	struct ui_data* ui = (struct ui_data*)arg;

	switch (ui->on_menu) {
	case SETTING:
		ui->on_menu = MAIN;
		break;
	case MODE:
		ui->on_menu = SETTING;
		break;
	case COLOR:
		ui->on_menu = SETTING;
		break;
	case COLOR_R:
	case COLOR_G:
	case COLOR_B:
		ui->on_menu = COLOR;
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

	ui::selc_reset(&ui->selc);
}

MAKE_CH_MENU_TRIG(handle_mode_selc, MODE);
MAKE_CH_MENU_TRIG(handle_color_selc, COLOR);

static void init_setting_menu(struct ui_data* ui) {
	struct ui_setting_menu* menu = &ui->setting_menu;

	ui::opt_elem_make(&menu->mode, "Mode", handle_mode_selc, ui);
	ui::opt_elem_make(&menu->color, "Color", handle_color_selc, ui);

	OPT_MAKE("Settings", 3, &menu->mode, &menu->color, &ui->back_butt);
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

	OPT_MAKE("Mode", 3, &menu->_auto, &menu->manual, &ui->back_butt);
}

MAKE_CH_MENU_TRIG(handle_r_selc, COLOR_R);
MAKE_CH_MENU_TRIG(handle_g_selc, COLOR_G);
MAKE_CH_MENU_TRIG(handle_b_selc, COLOR_B);

static void init_color_menu(struct ui_data* ui) {
	struct ui_color_menu* menu = &ui->color_menu;

	ui::opt_elem_make(&menu->r, "Red", handle_r_selc, ui);
	ui::opt_elem_make(&menu->g, "Green", handle_g_selc, ui);
	ui::opt_elem_make(&menu->b, "Blue", handle_b_selc, ui);

	OPT_MAKE("Color", 5, &menu->r, &menu->g, &menu->b, &ui->back_butt);
	ui::num_make(
		&menu->color_val,
		"Value",
		0,
		255,
		LCD_ROWS,
		&ui->selc,
		&ui->e,
		&screen
		);
}

MAKE_CH_MENU_TRIG(handle_toggle_selc, TOGGLE);

static void init_quickact_menu(struct ui_data* ui) {
	struct ui_quickact_menu* menu = &ui->quickact_menu;

	ui::opt_elem_make(&menu->toggle, "Toggle", handle_toggle_selc, ui);

	OPT_MAKE("Quickact", 2, &menu->toggle, &ui->back_butt);
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

	OPT_MAKE("Toggle", 3, &menu->on, &menu->off, &ui->back_butt);
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
	ui::opt_elem_make(&ui->back_butt, BACK_INDICATOR, back_to_prev_menu, ui);
	init_main_menu(ui);
	init_setting_menu(ui);
	init_mode_menu(ui);
	init_color_menu(ui);
	init_quickact_menu(ui);
	init_toggle_menu(ui);
	ui::opt_show(&ui->main_menu.opt);

	init_control(ui);

	ui->light = light;
}

static void set_color_val(struct ui_data* ui, u8 val) {
	switch (ui->on_menu) {
		case COLOR_R:
			light_set_color_r(ui->light, val);
			break;
		case COLOR_G:
			light_set_color_g(ui->light, val);
			break;
		case COLOR_B:
			light_set_color_b(ui->light, val);
			break;
		default:
			break;
	}
}

static void color_menu_loop(struct ui_data* ui) {
	struct ui_color_menu* menu = &ui->color_menu;

	if (ui->e != ui::NONE) {
		num_show(&menu->color_val);
		ui::event_take(&ui->e);
	}                                                                     
	if (num_loop(&menu->color_val)) {
		set_color_val(ui, ui::num_result(&menu->color_val));
		ui->on_menu = COLOR;
		ui::selc_reset(&ui->selc);
		ui::opt_show(&ui->color_menu.opt);
	}
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
	case COLOR:
		DEF_MENU_LOOP(&ui->color_menu);
		break;
	case COLOR_R:
	case COLOR_G:
	case COLOR_B:
		color_menu_loop(ui);
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
