#include "ldr.hpp"

namespace ldr {
	void make(ldr_t* ldr, u8 pin) {
		ldr->pin = pin;
		pinMode(pin, INPUT);
	}
}
