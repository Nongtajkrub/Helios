#pragma once

#include <stdint.h>
#include <math.h>

inline uint8_t min(uint8_t min, uint8_t val) {
	return (val < min) ? min : val;
}

inline uint16_t norm(uint16_t min, uint16_t max, uint16_t val) {
	return (uint16_t)round((float)(val - min) / (float)(max - min));
}
