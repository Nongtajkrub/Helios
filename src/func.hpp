#pragma once

#include <stdint.h>
#include <math.h>

template<typename int_type_ret, typename int_type_min, typename int_type_val>
inline int_type_ret min(int_type_min min, int_type_val val) {
	return (val < min) ? min : val;
}

template<typename int_type_ret, typename int_type_max, typename int_type_val>
inline int_type_ret max(int_type_max max, int_type_val val) {
	return (val > max) ? max : val;
}

template<typename int_type_ret, typename int_type_arr, typename int_type_size>
int_type_ret average_arr(const int_type_arr* arr, int_type_size size) {
	int_type_arr average = 0;

	for (uint8_t i = 0; i < size; i++) {
		average += arr[i];
	}

	return average / size;
}
