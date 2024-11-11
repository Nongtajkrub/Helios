#pragma once

#include "type.hpp"

typedef struct {
	u16 size;
	u16 add_i;

	u16 data_size;
	void** data;
} slwin_t;

void slwin_make(slwin_t* win, u16 size, u16 data_size);
void slwin_destroy(slwin_t* win);

void slwin_slide(slwin_t* win, void* data);

inline u16 slwin_cal_i(slwin_t* win, u16 i) {
	return (win->add_i + i) % win->size;
}

inline void* slwin_get(slwin_t* win, u16 i) {
	return win->data[slwin_cal_i(win, i)];
}

// get the last element of a sliding window
inline void* slwin_get_newest(slwin_t* win) {
	return slwin_get(win, win->size - 1);
}

inline void slwin_set(slwin_t* win, u16 i, void* val) {
	memcpy(win->data[slwin_cal_i(win, i)], val, win->data_size);
}

inline void slwin_set_newest(slwin_t* win, void* val) {
	memcpy(
		win->data[slwin_cal_i(win, win->size - 1)],
		val,
		win->data_size
		);
}
