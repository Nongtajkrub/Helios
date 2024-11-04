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

inline void* slwin_get(slwin_t* win, u16 get_i) {
	return win->data[(win->add_i + get_i) % win->size];
}

// get the last element of a sliding window
inline void* slwin_get_newest(slwin_t* win) {
	return slwin_get(win, win->size - 1);
}
