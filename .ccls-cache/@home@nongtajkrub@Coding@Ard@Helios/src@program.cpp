#include "program.hpp"

#define LDR_PIN 34

#define NP_PIN 26
#define NP_COUNT 1

#define LCD_ADDR 0x27
#define LCD_COLS 20
#define LCD_ROWS 4

#define LOOP_DELAY 50

#define UP_BUTT_PIN 12
#define DOWN_BUTT_PIN 14
#define SEL_BUTT_PIN 27

#define UI_WELCOM_TXT_ID 0
#define UI_SETTING_OPT_ID 1
#define UI_CONTROL_OPT_ID 2

namespace program {
	static void init_ui(struct data* data) {
		struct ui_data ui = data->ui;

		ui.on_menu = MAIN;

		ui.lcd = new I2C(LCD_ADDR, LCD_COLS, LCD_ROWS);
		ui.lcd->init();

		ui.main.welcom_txt  = ui::make("Welcome!", ui::TXT);
		ui.main.setting_opt = ui::make("Settings", ui::OPT);
		ui.main.control_opt = ui::make("Controls", ui::OPT);
		ui.main.group       = ui::group(
				ui.lcd,
				LCD_ROWS,
				LCD_COLS,
				3,
				ui.main.welcom_txt,
				ui.main.setting_opt,
				ui.main.control_opt
				);

		ui.up_button   = button::make(UP_BUTT_PIN);
		ui.down_button = button::make(DOWN_BUTT_PIN);
		ui.sel_button  = button::make(SEL_BUTT_PIN);
	}

	static void init_ldr(struct data* data) {
		data->ldr.ldr1 = ldr::make(LDR_PIN);
	}

	static void init_np(struct data* data) {
		data->np.pixel = np::make(NP_PIN, NP_COUNT);
	}

	void setup(struct data* data) {
		init_ui(data);
		init_ldr(data);
		init_np(data);
	}

	static void ui_loop(struct data* data) {
		struct ui_data ui = data->ui;

		switch (ui.on_menu) {
		case MAIN:
			ui::show(ui.main.group);
			break;
		case SETTING:
			break;
		}
	}

	void loop(struct data* data) {

	}
}
