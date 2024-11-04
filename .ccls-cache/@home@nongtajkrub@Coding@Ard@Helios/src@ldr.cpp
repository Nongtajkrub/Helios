#include "ldr.hpp"

namespace ldr {
	void make(ldr_t* ldr, u8 pin) {
		ldr->pin = pin;
		pinMode(pin, INPUT);

		slwin_make(&ldr->cache, LDR_CACHE_SIZE, sizeof(u16));
	}

	void cache_reading(ldr_t* ldr) {
		u16 reading = ldr::read(ldr);

		slwin_slide(&ldr->cache, (void*)&reading);
	}
}
