#pragma once

#include "ui_prog.hpp"

namespace program {
	struct data {
		struct ui_data* ui;
	};

	void init();
	void loop();
}
