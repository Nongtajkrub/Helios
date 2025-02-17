#pragma once

#include "neopixel.hpp"

#define GET_GLOBAL_SETTINGS
#include "settings.hpp"

namespace program {
struct light_data {
	np::pixel_t* pixels[NP_COUNT];
	u8 brightness[NP_COUNT];

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
void light_loop(struct light_data* light);

/* use for communicating with UI */

inline void light_set_mode_manu(struct light_data* light, bool val) {
	light->settings.mode.manual = val;
}

inline void light_set_on(struct light_data* light, bool val) {
	light->settings.mode.on = val;
}

inline bool light_is_manu(struct light_data* light) {
	return light->settings.mode.manual;
}

inline const u8* light_get_brightness(struct light_data* light) {
	return light->brightness;
}

inline void light_set_color_r(struct light_data* light, u8 val) {
	light->settings.color.r = val;
}

inline void light_set_color_g(struct light_data* light, u8 val) {
	light->settings.color.g = val;
}

inline void light_set_color_b(struct light_data* light, u8 val) {
	light->settings.color.b = val;
}
}
