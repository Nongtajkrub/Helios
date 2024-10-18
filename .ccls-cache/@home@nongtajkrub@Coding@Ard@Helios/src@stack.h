/*
 * This stack implementation was taken from another one of my C project
 * modified to work with embedded system
*/

#pragma once

#include <util/type.hpp>

typedef struct {
	u16 capacity;
	u16 size;

	u16 item_size;
	void** item;
} cstack_t;

void stack_make(cstack_t* stack, u16 item_size);
void stack_destroy(cstack_t* stack);

void stack_push(cstack_t* stack, void* push);
void* stack_pop(cstack_t* stack);

u16 stack_size(const cstack_t* stack);
