#pragma once

#include "ui.hpp"
#include "electronics.hpp"

namespace program {
	struct program_data {
		I2C lcd;
		ui::elem_t* welcom_txt;
		ui::elem_t* setting_opt;
		ui::group_t* ui_group;

		ldr::ldr_t* ldr1;

		np::pixel_t* pixel;
	};

	void setup(struct program_data* data);
	void loop(struct program_data* data);
}
