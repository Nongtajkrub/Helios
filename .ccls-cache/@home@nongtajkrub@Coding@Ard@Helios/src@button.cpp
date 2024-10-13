#include "button.hpp"
#include <Arduino.h>

namespace button {
	butt_t* make(u8 pin) {
		butt_t* butt = (butt_t*)malloc(sizeof(butt_t));

		butt->pin = pin;
		butt->old_state = false;

		pinMode(pin, INPUT);

		return butt;
	}
	
	void destroy(butt_t* butt) {
		free(butt);
	}

	bool state(butt_t* butt) {
		bool ret_val;
		bool state = digitalRead(butt->pin);

		ret_val = (state && !butt->old_state); 
		butt->old_state = state;

		return ret_val;
	}
}
