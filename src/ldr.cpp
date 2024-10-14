#include "ldr.hpp"

namespace ldr {
	ldr_t* make(u8 pin) {
		ldr_t* ldr = (ldr_t*)malloc(sizeof(ldr_t));

		ldr->pin = pin;
		pinMode(pin, INPUT);

		return ldr;
	}
}
