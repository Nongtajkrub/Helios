#include "light_prog.hpp"
#include "ui_prog.hpp"

#define GET_LIGHT_SETTINGS
#include "settings.hpp"

#include "func.hpp"
#include <Arduino.h>

namespace program {
	static void set_all_color(struct light_data* light, u8 r, u8 g, u8 b) {
		for (u8 i = 0; i < NP_COUNT; i++) {
			np::color(&light->pixels[i], r, g, b);
		}
	}

	static void init_setting(struct light_data* light) {
		light->settings.color = {
			.r = DEF_NP_R,
			.g = DEF_NP_G,
			.b = DEF_NP_B
		};

		light->settings.mode = {
			.manual = false,
			.on = true 
		};
	}

	void light_init(struct light_data* light) {
		// init ldr
		for (u8 i = 0; i < LDR_COUNT; i++) {
			ldr::make(&light->ldrs[i], LDR_PINS[i]);
		}

		// init neopixel
		for (u8 i = 0; i < NP_COUNT; i++) {
			np::make(&light->pixels[i], NP_PINS[i], 1);
		}
		set_all_color(light, DEF_NP_R, DEF_NP_G, DEF_NP_B);
	
		// init setting
		init_setting(light);
	}

	static void set_all_brightness(
		struct light_data* light,
		u8 brightness
		) {
		// if brightness is turn down to 0 neopixel wont turn back on
		brightness = min(1, brightness);

		for (u8 i = 0; i < NP_COUNT; i++) {
			np::brightness(&light->pixels[i], brightness);
		}
	}

	static void manual_mode_loop(struct light_data* light) {
		if (light->settings.mode.on) {
			set_all_brightness(light, 100);
		} else {
			set_all_brightness(light, 0);
		}
	}

	// caching all ldr reading 
	static void cache_ldr_reading(struct light_data* light) {
		for (u8 i = 0; i < LDR_COUNT; i++) {
			ldr::cache_reading(&light->ldrs[i]);
		}
	}

	// this is unreadable
	static inline u8 cal_np_distance(u8 np1_i, u8 np2_i) {
		// |x1 - x2| + |y1 - y2|
		return abs(
			round((f32)np1_i / (f32)NP_ROWS) - round((f32)np2_i / (f32)NP_ROWS)
			) + abs(
				round(np1_i % NP_COLS) - (u8)round(np2_i % NP_COLS)
				);
	}

	// src_ldr_i is the index of the ldr that is not a neighbors 
	static u16 propagate_ldr_reading(struct light_data* light, u8 src_ldr_i) {
		u16 result = light->ldrs[src_ldr_i].read_cache;

		for (u8 i = 0; i < LDR_COUNT; i++) {
			if (i == src_ldr_i) {
				continue;
			}

			// calculate weight
			f32 w = (f32)cal_np_distance(src_ldr_i, i) * (f32)RPWM;

			// propagate reading
			result += (u16)round((f32)w * (f32)light->ldrs[i].read_cache);
		}

		return result;
	}

	// normalize reading between 0 - 100
	static u8 norm_reading() {
		// TODO: implement normalization
		return 0;
	}

	static inline u8 cal_brightness(struct light_data* light, u8 src_ldr_i) {
		return norm(1, 100, propagate_ldr_reading(light, src_ldr_i));
	}

	// inline because I want this to be very fast
	static inline void auto_mode_loop(struct light_data* light) {
		cache_ldr_reading(light);

		// calculate all np brightness and set them
		for (u8 i = 0; i < LDR_COUNT; i++) {
			u8 norm_val = cal_brightness(light, i);
			Serial.print(i);
			Serial.print(" brightness: ");
			Serial.println(norm_val);
			np::brightness(&light->pixels[i], norm_val);
		}
	}

	void light_loop(struct light_data* light) {
		if (light->settings.mode.manual) {
			manual_mode_loop(light);
		} else {
			auto_mode_loop(light);
		}
	}
}
