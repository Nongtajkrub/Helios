/*
 * This is a simplify version of my ctui lib modify
 * to be compatible with embedded system instead of
 * the terminal.
*/

#pragma once

#include <Arduino.h>

#include <util/type.hpp>
#include <LiquidCrystal_I2C/LiquidCrystal_I2C.h>

typedef LiquidCrystal_I2C I2C;

namespace ui {
	typedef enum : u8 {
		TEXT,
		OPT
	} type_t;

	// ui element
	typedef struct elem {
		type_t type;
		// the element count at the time of creation
		// it will be the row that the element is on
		u8 id;
		// what will be show on the screen
		char* text;
	} elem_t;

	typedef struct {
		// how many element exist in this group
		// dont decrease when an element is destroy
		u8 count;
		// what element is the selecter on
		// store as the id of the element
		u8 select;
		// lcd that will be use to show the ui 
		I2C* screen;

		// head of the element group
		elem_t** elems;
	} group_t;
	
	struct selector_in {
		u8 id;

		type_t type;
		const char* text;
	};

	elem_t* make(const char* text, type_t type);
	void destroy(elem_t* elem);

	group_t* group(I2C* screen, int count, ...);
	void ungroup(group_t* group);

	void selector_up(group_t* group);
	void selector_down(group_t* group);
	struct selector_in selector_on(group_t* group);

	void show(group_t* group);
}
