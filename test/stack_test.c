#include <raptiquax.h>
#include <storage/stack.h>
#include "test.h"

Stack *stack = NULL;

int setup_stack() {
    printf("\n=== Setting up stack for tests ===");
    stack = malloc(sizeof(Stack));
    stack->next = NULL;
    return 0;
}

int teardown_stack() {
    printf("\n=== Tearing down stack after tests ===\n");
    stack_free(stack);
    free(stack);
    return 0;
}

void test_stack_push_and_pop() {
    int data1 = 10, data2 = 20, data3 = 30;

    // Push elements onto the stack
    stack_push(stack, &data1);
    stack_push(stack, &data2);
    stack_push(stack, &data3);

    // Pop elements and verify order
    CU_ASSERT(*(int *)stack_pop(stack) == 30);
    CU_ASSERT(*(int *)stack_pop(stack) == 20);
    CU_ASSERT(*(int *)stack_pop(stack) == 10);

    // Stack should now be empty
    CU_ASSERT(stack_is_empty(stack));
}

void test_stack_is_empty() {
    // Stack should initially be empty
    CU_ASSERT(stack_is_empty(stack));

    int data = 42;
    stack_push(stack, &data);

    // Stack should no longer be empty
    CU_ASSERT(!stack_is_empty(stack));

    stack_pop(stack);

    // Stack should be empty again
    CU_ASSERT(stack_is_empty(stack));
}

int test() {
    CU_pSuite pSuite = NULL;

    pSuite = CU_add_suite("Stack", setup_stack, teardown_stack);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
        (NULL == CU_add_test(pSuite, "Push and Pop elements", test_stack_push_and_pop)) ||
        (NULL == CU_add_test(pSuite, "Check if stack is empty", test_stack_is_empty))
    ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    return 0;
}

int main(int argc, char **argv) {
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    test();

    CU_basic_set_mode(CU_BRM_VERBOSE);
    if (CUE_SUCCESS != CU_basic_run_tests()) {
        printf("Tests failed\n");
        return -1;
    }

    CU_cleanup_registry();

    return 0;
}