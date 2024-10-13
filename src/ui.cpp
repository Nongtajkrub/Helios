#define SELECT_INDICATOR '>'

#include "ui.hpp"

namespace ui {
	static constexpr u16 ELEM_SIZE     = sizeof(elem_t);
	static constexpr u16 GROUP_SIZE    = sizeof(group_t);
	static constexpr u16 ELEM_PTR_SIZE = sizeof(elem_t*);

	elem_t* make(const char* text, type_t type) {
		const u16 TEXT_SIZE = strlen(text) + 1;
		elem_t* elem = (elem_t*)malloc(ELEM_SIZE);
		
		elem->type = type;

		// id will be assing when put in group
		elem->id = 0;

		elem->text = (char*)malloc(TEXT_SIZE);
		memcpy(elem->text, text, TEXT_SIZE);

		return elem;
	}

	// this is only for freeing element memories
	void destroy(elem_t* elem) {
		if (elem == NULL) {
			return;
		}

		if (elem->text != NULL) {
			free(elem->text);
		}
		free(elem);
	}

	// use int for count to prevent argument promotion
	group_t* group(I2C* screen, int count, ...) {
		group_t* group = (group_t*)malloc(GROUP_SIZE);
		va_list elems;
		va_start(elems, count);

		group->count  = count;
		group->select = 0;
		group->screen = screen;
		group->elems  = (elem_t**)calloc(count, ELEM_PTR_SIZE);

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

		return group;
	}

	void ungroup(group_t* group) {
		if (group == NULL) {
			return;
		}

		free(group->elems);
		free(group);
	}

	void selector_up(group_t* group) {
		// move up untill find an option element
		// or untill reach top row
		for (i8 i = group->select; i >= 0; i--) {
			elem_t* elem = group->elems[i];

			if (elem->type == OPT) {
				group->select = elem->id;
			}
		}
	}

	void selector_down(group_t* group) {
		// move down untill find an option element
		// or untill reach bottom row
		for (u8 i = group->select; i != group->count; i++) {
			elem_t* elem = group->elems[i];

			if (elem->type == OPT) {
				group->select = elem->id;
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

	static void show_handle_text(
			group_t* group,
			elem_t* elem
		) {
		group->screen->setCursor(0, elem->id);
		group->screen->print(elem->text);
	}

	static void show_handle_opt(
			group_t* group,
			elem_t* elem
		) {
		group->screen->setCursor(0, elem->id);
		
		if (elem->id == group->select) {
			// print the selector indicator first
			group->screen->print(SELECT_INDICATOR);
			group->screen->setCursor(1, elem->id);
		}
		group->screen->print(elem->text);
	}

	void show(group_t* group) {
		group->screen->clear();

		for (u8 i = 0; i < group->count; i++) {
			elem_t* elem = group->elems[i];

			switch (elem->type) {
			case TEXT:
				show_handle_text(group, elem);
				break;
			case OPT:
				show_handle_opt(group, elem);
				break;
			}
		}
	}
}
