#pragma once

#include <util/type.hpp>
#include <Adafruit_NeoPixel/Adafruit_NeoPixel.h>

typedef Adafruit_NeoPixel Neopixel;

namespace neopixel {
	typedef struct {
		Neopixel neopixel;

		u8 pin;
		u16 count;
	} pixel_t;

	pixel_t* make(u8 pin, u16 count);
}
