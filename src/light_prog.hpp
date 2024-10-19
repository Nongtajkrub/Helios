#pragma once

#include "electronics.hpp"

namespace program {
	struct light_data {
		ldr::ldr_t ldr1;
		// TODO: Add ldr2 and ldr3 when ready
		//ldr::ldr_t ldr2;
		//ldr::ldr_t ldr3;

		np::pixel_t pixel1;

		struct {
			struct {
				u8 r;
				u8 g;
				u8 b;
			} color;

			struct {
				bool manual;
				bool on;
			} mode;
		} settings;
	};

	void light_init(struct light_data* light);
	void light_loop(struct light_data* light, struct ui_data* ui);
}
