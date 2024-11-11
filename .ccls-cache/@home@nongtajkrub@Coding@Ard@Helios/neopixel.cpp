#include "neopixel.hpp"

namespace neopixel {
	pixel_t* make(u8 pin, u16 count) {
		pixel_t* pixel = (pixel_t*)malloc(sizeof(pixel_t));
		
		pixel->neopixel = Neopixel(count, pin);

		pixel->pin = pin;
		pixel->count = count;

		return pixel;
	}
}
