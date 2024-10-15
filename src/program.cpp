#include "program.hpp"

#define LDR_PIN 34

#define NP_PIN 26
#define NP_COUNT 1

#define LCD_ADDR 0x27
#define LCD_COLS 20
#define LCD_ROWS 4

#define LOOP_DELAY 50

namespace ui {

}

namespace program {
	static void init_lcd(struct program_data* data) {
		data->lcd = I2C(LCD_ADDR, LCD_COLS, LCD_ROWS);
	}
	
	static void init_ui(struct program_data* data) {
		data->welcom_txt = ui::make("Welcome!", ui::TXT);
		data->setting_opt = ui::make("Settings", ui::OPT);
		data->ui_group = ui::group(
				&data->lcd,
				LCD_ROWS,
				LCD_COLS,
				2,
				data->welcom_txt,
				data->setting_opt
				);
	}
	
	static void init_ldr(struct program_data* data) {
		data->ldr1 = ldr::make(LDR_PIN);
	}

	static void init_np(struct program_data* data) {
		data->pixel = np::make(NP_PIN, NP_COUNT);
	}

	void setup(struct program_data* data) {
		init_lcd(data);
		init_ui(data);
		init_ldr(data);
		init_np(data);
	}

	static void ui_loop(struct program_data* data) {
		ui::show(data->ui_group);
	}

	void loop(struct program_data* data) {
		delay(LOOP_DELAY);

		ui_loop(data);
	}
};
