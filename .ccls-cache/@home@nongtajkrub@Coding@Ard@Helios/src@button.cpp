#include "button.hpp"
#include <Arduino.h>

namespace button {
	void make(butt_t* butt, u8 pin) {
		butt->pin = pin;
		butt->old_state = false;

		pinMode(pin, INPUT);
	}
	
	void destroy(butt_t* butt) {
		pinMode(butt->pin, OUTPUT);
	}

	bool state(butt_t* butt) {
		bool ret_val;
		bool state = digitalRead(butt->pin);

		ret_val = (state && !butt->old_state); 
		butt->old_state = state;

		return ret_val;
	}
}
