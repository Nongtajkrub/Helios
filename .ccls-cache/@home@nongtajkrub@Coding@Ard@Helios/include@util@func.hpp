#pragma once

#include <stdint.h>

inline uint8_t min(uint8_t min, uint8_t val) {
	return (val < min) ? min : val;
}
