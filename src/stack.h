/*
 * This stack implementation was taken from another one of my C project
 * modified to work with embedded system
*/

#pragma once

#include <stdint.h>

typedef uint16_t u16;

typedef struct {
	u16 capacity;
	u16 size;

	u16 item_size;
	void** item;
} stack_t;

void stack_make(stack_t* stack, u16 item_size);
void stack_destroy(stack_t* stack);

void stack_push(stack_t* stack, void* push);
void* stack_pop(stack_t* stack);

u16 stack_size(const stack_t* stack);
