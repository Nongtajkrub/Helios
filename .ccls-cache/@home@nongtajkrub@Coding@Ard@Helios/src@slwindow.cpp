#include "slwindow.hpp"

#include <stdlib.h>
#include <memory.h>

static void alloc_data(slwin_t* win) {
	win->data = (void**)calloc(win->size, sizeof(void*));

	for (u16 i = 0; i < win->size; i++) {
		win->data[i] = malloc(win->data_size);
		memset(win->data[i], 0, win->data_size);
	}
}

void slwin_make(slwin_t* win, u16 size, u16 data_size) {
	win->size = size;
	win->add_i = 0;

	win->data_size = data_size;

	alloc_data(win);
}

void slwin_destroy(slwin_t* win) {
	for (u8 i = 0; i < win->size; i++) {
		free(win->data[i]);
	}
	free(win->data);
}

void slwin_slide(slwin_t* win, void* data) {
	if (win->add_i != 0 && win->add_i % win->size == 0) {
		win->add_i = 0;
	}

	memcpy(win->data[win->add_i], data, win->data_size);
	win->add_i++;
}
