#pragma once

#include <util/type.hpp>
#include <Arduino.h>

namespace ldr {
	typedef struct {
		u8 pin;
	} ldr_t;

	// pointer is bigger than the ldr_t itself but who care
	ldr_t* make(u8 pin);

	inline u16 read(ldr_t* ldr) {
		return analogRead(ldr->pin);
	}
}
