#include "neopixel.hpp"

namespace np {
	void make(pixel_t* pixel, u8 pin, u16 count) {
		pixel->neopixel = Neopixel(count, pin);
		pixel->neopixel.begin();

		pixel->count = count;
	}

	void destroy(pixel_t* pixel) {
		pixel->neopixel.clear();
	}
	
	void brightness(pixel_t* pixel, u8 brightness) {
		pixel->neopixel.clear();
		pixel->neopixel.setBrightness(brightness);
		pixel->neopixel.show();
	}

	void color(pixel_t* pixel, u8 r, u8 g, u8 b) {
		pixel->neopixel.clear();
		for (u16 i = 0; i < pixel->count; i++) {
			pixel->neopixel.setPixelColor(i, r, g, b);
		}
		pixel->neopixel.show();
	}

	void color(pixel_t* pixel, u16 i, u8 r, u8 g, u8 b) {
		pixel->neopixel.clear();
		pixel->neopixel.setPixelColor(i, r, g, b);
		pixel->neopixel.show();
	}
}
