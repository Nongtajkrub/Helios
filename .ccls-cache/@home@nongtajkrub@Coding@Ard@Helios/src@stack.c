#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define DEF_CAPACITY 4

static const u16 STACK_SIZE = sizeof(cstack_t);

void stack_make(cstack_t* stack, u16 item_size) {
	stack->capacity = DEF_CAPACITY;
	stack->size = 0;

	stack->item_size = item_size;
	stack->item = (void**)calloc(DEF_CAPACITY, PTR_SIZE);
}

void stack_destroy(cstack_t* stack) {
	for (u16 i = 0; i < stack->size; i++) {
		if (stack->item != NULL) {
			free(stack->item[i]);
		}
	}
	free(stack->item);
}

void stack_push(cstack_t* stack, void* push) {
	// stack full
	if (stack->size != 0 && stack->size % stack->capacity == 0) {
		// increase capacity in fixed chunks
		stack->capacity = (stack->size / DEF_CAPACITY + 1) * DEF_CAPACITY;
		stack->item = (void**)realloc(stack->item, stack->capacity * PTR_SIZE);
	}

	stack->item[stack->size] = malloc(stack->item_size);
	memcpy(stack->item[stack->size], push, stack->item_size);
	stack->size++;
}

void* stack_pop(cstack_t* stack) {
	if (stack->size == 0) {
		return NULL;
	}
	// if not size - 1 will get segmentation fault 
	void* ret_val = stack->item[stack->size - 1];
	stack->size--;

	return ret_val;
}

u16 stack_size(const cstack_t* stack) {
	return stack->size;
}
