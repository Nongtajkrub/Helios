#pragma once

#include "type.hpp"
#include <Arduino.h>

namespace ldr {
	typedef struct {
		u8 pin;

		u16 read_cache;
	} ldr_t;

	void make(ldr_t* ldr, u8 pin);
	// read and return reading
	inline u16 read(ldr_t* ldr) {
		return analogRead(ldr->pin);
	}

	// read and cache the reading in read_cache
	inline void cache_reading(ldr_t* ldr) {
		ldr->read_cache = ldr::read(ldr);
	}
}
