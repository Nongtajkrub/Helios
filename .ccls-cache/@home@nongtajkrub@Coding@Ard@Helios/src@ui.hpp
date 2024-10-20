/*
 * this is a simplify version of my ctui lib modify
 * to be compatible with embedded system
*/

#pragma once

#include <Arduino.h>

#include <util/type.hpp>
#include <LiquidCrystal_I2C/LiquidCrystal_I2C.h>

#define NO_WRAP 0

typedef LiquidCrystal_I2C I2C;

namespace ui {
	typedef enum : u8 {
		TXT,
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
		u8 rows;
		u8 cols;

		// elements in element group
		elem_t** elems;
	} group_t;

	struct selector_in {
		u8 id;

		type_t type;
		const char* text;
	};

	void make(elem_t* elem, const char* text, type_t type);
	void destroy(elem_t* elem);

	void group(group_t* group, I2C* screen, u8 rows, u8 cols, int count, ...);
	void ungroup(group_t* group);

	void selector_up(group_t* group);
	void selector_down(group_t* group);
	struct selector_in selector_on(group_t* group);

	// show plain txt with wraping
	void show(const char* text, I2C* screen, u8 x, u8 y, u8 wrap_at);
	// show group of element does not support wraping yet
	void show(group_t* group);
}
