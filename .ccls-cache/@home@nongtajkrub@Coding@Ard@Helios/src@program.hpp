#pragma once

#include "ui_prog.hpp"
#include "light_prog.hpp"

namespace program {
	struct data {
		struct ui_data ui;
		struct light_data light;
	};

	void setup(struct data* data);
	void loop(struct data* data);
}
