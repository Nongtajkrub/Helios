#include "program.hpp"

#define LDR_PIN 34

#define NP_PIN 26
#define NP_COUNT 1

#define LOOP_DELAY 50

namespace program {
	void setup(struct data* data) {
		ui_init(&data->ui);
	}

	void loop(struct data* data) {
		ui_loop(&data->ui);
		delay(LOOP_DELAY);
	}
}
