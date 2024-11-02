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
		//set_all_color(light, DEF_NP_R, DEF_NP_G, DEF_NP_B);
		np::color(&light->pixels[0], 255, 0, 0);
		np::color(&light->pixels[1], 0, 255, 0);
		np::color(&light->pixels[2], 0, 0, 255);
		np::color(&light->pixels[3], 255, 255, 255);

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
			Serial.print("LDR: ");
			Serial.println(ldr::read(&light->ldrs[i]));
			ldr::cache_reading(&light->ldrs[i]);
		}
	}

	// this is unreadable
	static inline u8 cal_np_distance(u8 np1_i, u8 np2_i) {
		// |x1 - x2| + |y1 - y2|
		return fabs(
			(np1_i % NP_COLS) - (np2_i % NP_COLS)
			) + fabs(
				(floor((f32)np1_i / (f32)NP_COLS)) - (floor((f32)np2_i / (f32)NP_COLS))
				);
	}

	// normalize all reading to be between 0 - 100
	static void norm_readings(struct light_data* light) {	
		for (u8 i = 0; i < LDR_COUNT; i++) {
			ldr::set_cache(
				&light->ldrs[i],
				(u8)round((f32)ldr::get_cache(&light->ldrs[i]) / (f32)RNF)
				);
		}
	}

	// src_ldr_i is the index of the ldr that is not a neighbors 
	static void propagate_readings(struct light_data* light) {
		for (u8 src = 0; src < LDR_COUNT; src++) {
			u8 result = ldr::get_cache(&light->ldrs[src]);

			// propagate reading
			for (u8 nbr = 0; nbr < LDR_COUNT; nbr++) {
				// skip the current src
				if (nbr == src) {
					continue;
				}

				// compute weight
				f32 w = (f32)RPWM / (f32)cal_np_distance(src, nbr);

				// compute result
				result += (u8)round(w * (f32)ldr::get_cache(&light->ldrs[nbr]));
			}

			// cap the result to 100 and cache it 
			ldr::set_cache(&light->ldrs[src], max(100, result));
		}
	}

	// turn pure AO from LDR to brightness for each neopixel ranging between
	// 0 - 100 calculation result get store in ldr cache
	static inline void cal_brightness(struct light_data* light) {
		cache_ldr_reading(light);
		norm_readings(light);
		propagate_readings(light);
	}

	// inline because I want this to be very fast
	static inline void auto_mode_loop(struct light_data* light) {
		// calculate brightness and store them in the ldrs cache
		cal_brightness(light);

		for (u8 i = 0; i < NP_COUNT; i++) {
			//Serial.print("brightness: ");
			//Serial.println(ldr::get_cache(&light->ldrs[i]));

			np::brightness(&light->pixels[i], ldr::get_cache(&light->ldrs[i]));
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
