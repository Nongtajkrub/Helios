#pragma once

#include "type.hpp"

namespace button {
	typedef struct {
		u8 pin;

		bool old_state;
	} butt_t;

	void make(butt_t* butt, u8 pin);
	void destroy(butt_t* butt);

	bool state(butt_t* butt); 
}
