#pragma once

#include "ui_prog.hpp"
#include "light_prog.hpp"
#include "netpie.hpp"

#include <Arduino.h>

namespace program {
	struct data {
		struct light_data light;
		struct ui_data ui;
		struct netpie_data netpie;
	};

	void setup(struct data* data);
	void loop(struct data* data);
}
