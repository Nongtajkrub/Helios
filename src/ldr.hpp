#pragma once

#include "type.hpp"
#include <Arduino.h>

namespace ldr {
	typedef struct {
		u8 pin;

		u16 cache;
	} ldr_t;

	void make(ldr_t* ldr, u8 pin);
	// read and return reading
	inline u16 read(ldr_t* ldr) {
		return analogRead(ldr->pin);
	}

	// read and cache the reading in read_cache
	inline void cache_reading(ldr_t* ldr) {
		ldr->cache = ldr::read(ldr);
	}

	inline u16 get_cache(const ldr_t* ldr) {
		return ldr->cache;
	}

	inline void set_cache(ldr_t* ldr, u16 val) {
		ldr->cache = val;
	}
}
