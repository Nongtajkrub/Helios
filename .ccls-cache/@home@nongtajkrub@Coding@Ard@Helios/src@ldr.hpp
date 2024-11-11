#pragma once

#include "type.hpp"
#include "slwindow.hpp"
#include <Arduino.h>

#define LDR_CACHE_SIZE 4

namespace ldr {
	typedef struct {
		u8 pin;

		slwin_t cache;
	} ldr_t;

	void make(ldr_t* ldr, u8 pin);

	// read and cache the reading
	void cache_reading(ldr_t* ldr);

	// read and return reading
	inline u16 read(ldr_t* ldr) {
		return analogRead(ldr->pin);
	}

	// return the most recent cache
	inline u16 get_cache(ldr_t* ldr) {
		return *(u16*)slwin_get_newest(&ldr->cache);
	}

	inline u16 get_cache(ldr_t* ldr, u8 i) {
		return *(u16*)slwin_get(&ldr->cache, 1);
	}

	inline void set_cache(ldr_t* ldr, void* val) {
		slwin_set_newest(&ldr->cache, val);
	}

	inline void set_cache(ldr_t* ldr, u8 i, void* val) {
		slwin_set(&ldr->cache, i, val);
	}
}
