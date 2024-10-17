#include "stack.h"
#include <stdlib.h>
#include <memory.h>

#define DEF_CAPACITY 4

static const u16 STACK_SIZE = sizeof(stack_t);

void stack_make(stack_t* stack, u16 item_size) {
	stack->capacity = DEF_CAPACITY;
	stack->size = 0;

	stack->item_size = item_size;
	stack->item = calloc(item_size, DEF_CAPACITY);
}

void stack_destroy(stack_t* stack) {
	if (stack == NULL) {
		return;
	}

	for (u16 i = 0; i < stack->capacity; i++) {
		if (stack->item != NULL) {
			free(stack->item[i]);
		}
	}
}

void stack_push(stack_t* stack, void* new) {
	// stack full
	if (stack->size % stack->capacity == 0) {
		// increase capacity in fixed chunks
		stack->capacity = (stack->size / DEF_CAPACITY + 1) * DEF_CAPACITY;
		stack->item = realloc(stack->item, stack->capacity * stack->item_size);
	}

	memcpy(stack->item[stack->size], new, stack->item_size);
	stack->size++;
}
