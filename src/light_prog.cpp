#include "light_prog.hpp"
#include "ui_prog.hpp"
#include "ldr.hpp"

#define GET_DEBUG_SETTINGS 
#define GET_LIGHT_SETTINGS
#include "settings.hpp"

#include "func.hpp"
#include <Arduino.h>

namespace program {
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

	static void set_all_color(struct light_data* light, u8 r, u8 g, u8 b) {
		for (u8 i = 0; i < NP_COUNT; i++) {
			np::color(light->pixels[i], r, g, b);
			np::show(light->pixels[i]);
		}
	}

	void light_init(struct light_data* light) {
		// init ldr
		for (u8 i = 0; i < LDR_COUNT; i++) {
			ldr::make(LDR_PINS[i]);
		}

		// init neopixel
		for (u8 i = 0; i < NP_COUNT; i++) {
			np::make(light->pixels[i], NP_PINS[i], 1);
			np::begin(light->pixels[i]);
		}
		set_all_color(light, DEF_NP_R, DEF_NP_G, DEF_NP_B);

		// init setting
		init_setting(light);
	}

	static void set_brightness(struct light_data* light, u8 i, u8 brightness) {
		// if brightness is turn down to 0 neopixel wont turn back on
		np::brightness(light->pixels[i], min<u8, u8, u8>(1, brightness));
		np::show(light->pixels[i]);
	}

	static void set_brightness(struct light_data* light, u8 brightness) {
		// if brightness is turn down to 0 neopixel wont turn back on
		brightness = min<u8, u8, u8>(1, brightness);

		for (u8 i = 0; i < NP_COUNT; i++) {
			np::brightness(light->pixels[i], brightness);
			np::show(light->pixels[i]);
		}
	}

	// normalize all reading to be between 1 - 100
	static void reading_to_brightness(struct light_data* light) {	
		for (u8 i = 0; i < LDR_COUNT; i++) {
		#if INVERT_SIGNAL == 1
			light->brightness[i] = max<u8, u8, u8>(
				100,
				(u8)round((f32)(LDR_MAX_ADC - ldr::read(LDR_PINS[i])) / (f32)RTBNF)
				);
		#else
			light->brightness[i] = max<u8, u8, u8>(
				100,
				(u8)round((f32)ldr::read(LDR_PINS[i]) / (f32)RTBNF)
				);
		#endif // #ifdef INVERT_SIGNAL
		}
	}

	// calculate distance between 2 neopixel (this is unreadable)
	static inline u8 cal_np_distance(u8 np1_i, u8 np2_i) {
		// |x1 - x2| + |y1 - y2|
		return fabs(
			(np1_i % NP_COLS) - (np2_i % NP_COLS)
			) + fabs(
				(floor((f32)np1_i / (f32)NP_COLS)) - (floor((f32)np2_i / (f32)NP_COLS))
				);
	}

	// src_ldr_i is the index of the ldr that is not a neighbors
	static void propagate_brightness(struct light_data* light) {
		for (u8 src = 0; src < LDR_COUNT; src++) {
			u8 result = light->brightness[src];

			// propagate reading
			for (u8 nbr = 0; nbr < LDR_COUNT; nbr++) {
				// skip the current src
				if (nbr == src) {
					continue;
				}

				// compute weight
				f32 w = (f32)BPWM / (f32)cal_np_distance(src, nbr);

				// compute result
				result += (u8)round(w * (f32)light->brightness[nbr]);
			}

			// cap the result to 100 and cache it 
			light->brightness[src] = max<u8, u8, u8>(100, result);
		}
	}

	// inline because I want this to be very fast
	static inline void auto_mode_loop(struct light_data* light) {
		reading_to_brightness(light);
	#if NP_COUNT > 1
		propagate_brightness(light);
	#endif // #if NP_COUNT != 1

		for (u8 i = 0; i < NP_COUNT; i++) {
			set_brightness(light, i, light->brightness[i]);
		}
	}

	static void manual_mode_loop(struct light_data* light) {
		if (light->settings.mode.on) {
			set_brightness(light, 100);
		} else {
			set_brightness(light, 0);
		}
	}

	// reset color needed because changing brightness is lossy and will
	// lose color data
	static void reset_color(struct light_data* light) {
		set_all_color(
			light,
			light->settings.color.r,
			light->settings.color.g,
			light->settings.color.b
			);
	}

	void light_loop(struct light_data* light) {
		if (!light->settings.mode.manual) {
			auto_mode_loop(light);
		} else {
			manual_mode_loop(light);
		}

		reset_color(light);
	}
}
