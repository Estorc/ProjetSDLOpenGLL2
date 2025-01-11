#pragma once

/**
 * @brief Stack data structure and its associated operations.
 *
 * This file contains the definition of a stack data structure and the 
 * declarations of functions to manipulate the stack. The stack is implemented 
 * as a linked list, where each element points to the next element in the stack.
 * The stack supports basic operations such as checking if it is empty, pushing 
 * an element onto the stack, popping an element from the stack, and freeing the 
 * stack.
 *
 * @defgroup Stack Stack Data Structure
 * @{
 */

/**
 * @struct Stack
 * @brief A node in the stack.
 *
 * This structure represents a single node in the stack. Each node contains a 
 * pointer to the next node in the stack and a pointer to the data stored in 
 * the node.
 */
typedef struct Stack {
    struct Stack *next; /**< Pointer to the next node in the stack. */
    void *data; /**< Pointer to the data stored in the node. */
} Stack;

/**
 * @brief Checks if the stack is empty.
 *
 * This function checks whether the given stack is empty. A stack is considered 
 * empty if the pointer to the stack is NULL.
 *
 * @param stack Pointer to the stack.
 * @return true if the stack is empty, false otherwise.
 */

bool stack_is_empty(Stack *stack);

/**
 * @brief Pushes an element onto the stack.
 *
 * This function pushes a new element onto the top of the stack. The new element 
 * contains the given data and becomes the new top of the stack.
 *
 * @param stack Pointer to the stack.
 * @param data Pointer to the data to be pushed onto the stack.
 */

void stack_push(Stack *stack, void *data);

/**
 * @brief Pops an element from the stack.
 *
 * This function removes the top element from the stack and returns the data 
 * stored in that element. If the stack is empty, the function returns NULL.
 *
 * @param stack Pointer to the stack.
 * @return Pointer to the data stored in the popped element, or NULL if the 
 * stack is empty.
 */

void *stack_pop(Stack *stack);

/**
 * @brief Frees the stack.
 *
 * This function frees all the elements in the stack and the data stored in 
 * each element. After calling this function, the stack pointer will be NULL.
 *
 * @param stack Pointer to the stack.
 */

void stack_free(Stack *stack);

/** @} */ // end of Stack group