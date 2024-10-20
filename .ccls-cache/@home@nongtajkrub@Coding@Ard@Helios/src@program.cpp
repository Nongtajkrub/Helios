#include "program.hpp"

#define LOOP_DELAY 50

namespace program {
	void setup(struct data* data) {
		ui_init(&data->ui, &data->light);
		light_init(&data->light);
	}

	void loop(struct data* data) {
		ui_loop(&data->ui);
		light_loop(&data->light, &data->ui);
		delay(LOOP_DELAY);
	}
}
