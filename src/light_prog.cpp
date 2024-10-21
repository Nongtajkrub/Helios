#include "light_prog.hpp"
#include "ui_prog.hpp"

#define GET_LIGHT_SETTINGS
#include "settings.hpp"

#include <util/func.h>
#include <Arduino.h>

namespace program {
	static void init_ldr(struct light_data* light) {
		ldr::make(&light->ldr1, LDR1_PIN);
		// TODO: Add ldr2 and ldr3 when ready
	}

	static void init_np(struct light_data* light) {
		np::make(&light->pixel1, NP_PIN, NP_COUNT);
		np::color(&light->pixel1, DEF_NP_R, DEF_NP_G, DEF_NP_B);
	}

	static void init_setting(struct light_data* light) {
		light->settings.color = {
			.r = DEF_NP_R,
			.g = DEF_NP_G,
			.b = DEF_NP_B
		};

		light->settings.mode = {
			.manual = true,
			.on = true 
		};
	}

	void light_init(struct light_data* light) {
		init_ldr(light);
		init_np(light);
		init_setting(light);
	}

	struct brightness {
		u8 pixel1;
	};

	static struct brightness calculate_brightness(struct light_data* light) {
		// map ldr values to 1 - 100 brightness cant be 0
		struct brightness ret_val = {
			.pixel1 = min(1, (u8)round((f32)ldr::read(&light->ldr1) / BNF))
		};

		return ret_val;
	}

	static inline void set_all_brightness(
		struct light_data* light,
		u8 brightness
		) {
		// if brightness is turn down to 0 neopixel wont turn back on
		brightness = min(1, brightness);

		np::brightness(&light->pixel1, brightness);
	}

	static void manual_mode_loop(struct light_data* light) {
		if (light->settings.mode.on) {
			set_all_brightness(light, 100);
		} else {
			set_all_brightness(light, 0);
		}
	}

	static void auto_mode_loop(struct light_data* light) {
		struct brightness brightness = calculate_brightness(light);

		np::brightness(&light->pixel1, brightness.pixel1);
	}

	void light_loop(struct light_data* light, struct ui_data* ui) {
		if (light->settings.mode.manual) {
			manual_mode_loop(light);
		} else {
			auto_mode_loop(light);
		}
	}
}
