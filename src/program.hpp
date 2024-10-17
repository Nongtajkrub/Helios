#pragma once

#include "ui_prog.hpp"

namespace program {
	struct data {
		struct ui_data ui;
	};

	void setup(struct data* data);
	void loop(struct data* data);
}
