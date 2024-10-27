#include "ui.hpp"

#define SELECT_INDICATOR '>'

namespace ui {
	static constexpr u16 ELEM_SIZE = sizeof(elem_t);
	static constexpr u16 GROUP_SIZE = sizeof(group_t);
	static constexpr u16 ELEM_PTR_SIZE = sizeof(elem_t*);

	void make(elem_t* elem, const char* text, type_t type) {
		const u16 TEXT_SIZE = strlen(text) + 1;

		elem->type = type;

		// id will be assing when put in group
		elem->id = 0;

		elem->text = (char*)malloc(TEXT_SIZE);
		memset(elem->text, '\0', TEXT_SIZE);
		memcpy(elem->text, text, TEXT_SIZE);
	}

	// this is only for freeing element memories
	void destroy(elem_t* elem) {
		if (elem == NULL) {
			return;
		}

		if (elem->text != NULL) {
			free(elem->text);
		}
	}

	// use int for count to prevent argument promotion
	void group(group_t* group, I2C* screen, u8 rows, u8 cols, int count, ...) {
		va_list elems;
		va_start(elems, count);

		group->count = count;
		group->select = 0;
		group->screen = screen;
		screen->backlight();
		group->rows = rows;
		group->cols = cols;
		group->elems = (elem_t**)calloc(count, ELEM_PTR_SIZE);

		// add all element to the group
		for (u8 i = 0; i < count; i++) {
			group->elems[i] = va_arg(elems, elem_t*);
			group->elems[i]->id = i;
		}
		va_end(elems);

		// move select indicator to the first option element
		// in the group
		if (group->elems[0]->type != OPT) {
			selector_down(group);
		}
	}

	void ungroup(group_t* group) {
		if (group == NULL) {
			return;
		}

		free(group->elems);
		free(group);
	}

	void selector_up(group_t* group) {
		// move up skipping the currently selected option 
		// untill find an option or untill reach top row
		for (i8 i = group->select - 1; i >= 0; i--) {
			elem_t* elem = group->elems[i];

			if (elem->type == OPT) {
				group->select = elem->id;
				return;
			}
		}
	}

	void selector_down(group_t* group) {
		// move down skipping the currently selected option 
		// untill find an option or untill reach bottom row
		for (u8 i = group->select + 1; i != group->count; i++) {
			elem_t* elem = group->elems[i];

			if (elem->type == OPT) {
				group->select = elem->id;
				return;
			}
		}
	}

	struct selector_in selector_on(group_t* group) {
		elem_t* elem = group->elems[group->select];

		struct selector_in info = {
			.id   = group->select,
			.type = elem->type,
			.text = elem->text
		};

		return info;
	}

	void show(const char* text, I2C* screen, u8 x, u8 y, u8 wrap_at) {
		const u8 text_size = strlen(text);

		screen->clear();

		// print without wraping
		if (wrap_at == NO_WRAP) {
			screen->print(text);
			return;
		}

		// print with wraping
		u8 current_y = 0;
		u8 current_x = 0;
		for (u8 i = x; i < text_size; i++) {
			if (current_x >= wrap_at) {
				current_x = 0;
				current_y++;
			}

			screen->setCursor(current_x, current_y);
			screen->print(text[i]);

			current_x++;
		}
	}

	static void show_handle_txt(u8 line, group_t* group, elem_t* elem) {
		group->screen->setCursor(0, line);
		group->screen->print(elem->text);
	}

	static void show_handle_opt(u8 line, group_t* group, elem_t* elem) {
		group->screen->setCursor(0, line);
		
		if (elem->id == group->select) {
			// print the selector indicator first
			group->screen->print(SELECT_INDICATOR);
			group->screen->setCursor(1, line);
		}
		group->screen->print(elem->text);
	}

	void show(group_t* group) {
		u8 start = 0;
		u8 line = 0;

		group->screen->clear();

		// scrolling
		if (group->select >= group->rows) {
			start += (group->select - group->rows) + 1;
		}

		for (u8 i = start; i < group->count && i < (group->rows + start); i++) {
			elem_t* elem = group->elems[i];

			switch (elem->type) {
			case TXT:
				show_handle_txt(line, group, elem);
				break;
			case OPT:
				show_handle_opt(line, group, elem);
				break;
			}
			line++;
		}
	}
}
