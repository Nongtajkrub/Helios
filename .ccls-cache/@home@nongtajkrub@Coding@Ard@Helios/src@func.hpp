#pragma once

#include <stdint.h>
#include <math.h>

inline uint8_t min(uint16_t min, uint16_t val) {
	return (val < min) ? min : val;
}

inline uint8_t max(uint16_t max, uint16_t val) {
	return (val > max) ? max : val;
}
