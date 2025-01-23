#line 1 "src/storage/stack.c"
#include "../raptiquax.h"
#include "stack.h"

void stack_push(Stack *stack, void *data) {
    Stack *new_element = malloc(sizeof(Stack));
    new_element->data = data;
    new_element->next = stack->next;
    stack->next = new_element;
}

bool stack_is_empty(Stack *stack) {
    return stack->next == NULL;
}

void *stack_pop(Stack *stack) {
    void * data = stack->next->data;
    Stack *old = stack->next;
    stack->next = stack->next->next;
    free(old);
    return data;
}

void stack_free(Stack *stack) {
    Stack *cursor = stack->next;
    while (cursor->next != NULL) {
        Stack *next = cursor->next;
        free(cursor);
        cursor = next;
    }
}