// Just an alias

#pragma once

#include "type.hpp"
#include <Arduino.h>

namespace ldr {
	inline void make(u8 pin) {
		pinMode(pin, INPUT);
	}

	inline u16 read(u8 pin) {
		return analogRead(pin);
	}
}
