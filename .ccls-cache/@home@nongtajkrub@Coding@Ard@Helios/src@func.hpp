#pragma once

#include <stdint.h>
#include <math.h>

inline uint16_t min(uint16_t min, uint16_t val) {
	return (val < min) ? min : val;
}

inline uint16_t max(uint16_t max, uint16_t val) {
	return (val > max) ? max : val;
}
