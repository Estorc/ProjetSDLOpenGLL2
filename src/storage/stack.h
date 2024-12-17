typedef struct Stack {
    struct Stack *next;
    void *data;
} Stack;

bool stack_is_empty(Stack *stack);
void stack_push(Stack *stack, void *data);
void *stack_pop(Stack *stack);
void stack_free(Stack *stack);