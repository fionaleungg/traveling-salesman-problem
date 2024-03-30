#include "stack.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct stack {
    uint32_t capacity;
    uint32_t top;
    uint32_t *items;
} Stack;

Stack *stack_create(uint32_t capacity) {
    // Attempt to allocate memory for a stack
    // Cast it to a stack pointer too!
    Stack *s = (Stack *) malloc(sizeof(Stack));
    s->capacity = capacity;
    s->top = 0;
    // We need enough memory for <capacity> numbers
    s->items = calloc(s->capacity, sizeof(uint32_t));
    // We created our stack, return it!
    return s;
}

void stack_free(Stack **sp) {
    // sp is a double pointer, so we have to check if it,
    // or the pointer it points to is null.
    if (sp != NULL && *sp != NULL) {
        // Of course, we have to remember to free the
        // memory for the array of items first,
        // as that was also dynamically allocated!
        // If we freed the Stack first then we would
        // not be able to access the array to free it.
        if ((*sp)->items) {
            free((*sp)->items);
            (*sp)->items = NULL;
        }
        // Free memory allocated for the stack
        free(*sp);
    }
    if (sp != NULL) {
        // Set the pointer to null! This ensures we dont ever do a double free!
        *sp = NULL;
    }
}

bool stack_push(Stack *s, uint32_t val) {
    // If the stack is full, return false;
    if (stack_full(s)) {
        return false;
    }
    // Set val
    s->items[s->top] = val;
    // Move the top of the stack
    s->top++;
    return true;
}

bool stack_pop(Stack *s, uint32_t *val) {
    // if the stack is empty, pop is not possible --> return false
    if (stack_empty(s)) {
        return false;
    }
    // put the item at the top of stack into the address that val is pointing to
    // top - 1 because top is the NEXT empty item. i want the last item
    *val = s->items[(s->top) - 1];
    // modify stack
    s->top--;
    return true;
}

bool stack_peek(const Stack *s, uint32_t *val) {
    if (stack_empty(s)) {
        return false;
    }
    *val = s->items[(s->top) - 1];
    return true;
}

bool stack_empty(const Stack *s) {
    // a stack is empty if the next empty index is index 0
    if (s->top == 0) {
        return true;
    } else {
        return false;
    }
}

bool stack_full(const Stack *s) {
    // a stack is full if the next empty index is equal to the capacity
    if (s->top == s->capacity) {
        return true;
    } else {
        return false;
    }
}

uint32_t stack_size(const Stack *s) {
    return (s->top);
}

void stack_copy(Stack *dst, const Stack *src) {
    // set the top of the dst Stack to 0 to prepare for overwriting
    dst->top = 0;
    for (int i = 0; i < (int) (stack_size(src)); i++) {
        // although it is not supposed to happen, check if the assert statement below checks if the capacity
        // of the dst Stack is less than the capcity of the src Stack. if it is, the call to stack_push() will
        // return false and the assertion will throw an assertion error

        assert(stack_push(dst, src->items[i]));
    }
}

void stack_print(const Stack *s, FILE *outfile, char *cities[]) {
    for (uint32_t i = 0; i < s->top; i += 1) {
        fprintf(outfile, "%s\n", cities[s->items[i]]);
    }
}
