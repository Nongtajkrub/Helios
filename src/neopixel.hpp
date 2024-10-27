#pragma once

#include "type.hpp"
#include <Adafruit_NeoPixel/Adafruit_NeoPixel.h>

typedef Adafruit_NeoPixel Neopixel;
extern bool np_auto_show;

namespace np {
	typedef struct {
		Neopixel* neopixel;

		u16 count;
		u8 brightness;
	} pixel_t;

	void make(pixel_t* pixel, u8 pin, u16 count);
	void destroy(pixel_t* pixel);

	void brightness(pixel_t* pixel, u8 brightness);

	void color(pixel_t* pixel, u8 r, u8 g, u8 b);
	void color(pixel_t* pixel, u16 i, u8 r, u8 g, u8 b);
}
