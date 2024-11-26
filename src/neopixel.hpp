// alias for Adafruit_NeoPixel to let me write neopixel code C style

#pragma once

#include "type.hpp"
#include <Adafruit_NeoPixel.h>

namespace np {
typedef Adafruit_NeoPixel pixel_t;

inline void make(pixel_t*& pixel, u8 pin, u8 count) {
	pixel = new Adafruit_NeoPixel(count, pin);
}

inline void begin(pixel_t* pixel) {
	pixel->begin();
}

inline void color(pixel_t* pixel, u8 r, u8 g, u8 b) {
	pixel->fill(pixel->Color(r, g, b));
}

inline void color(pixel_t* pixel, u16 i, u8 r, u8 g, u8 b) {
	pixel->setPixelColor(i, r, g, b);
}

inline void brightness(pixel_t* pixel, u8 brightness) {
	pixel->setBrightness(brightness);
}

inline void show(pixel_t* pixel) {
	pixel->show();
}
}
