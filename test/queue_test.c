#include <raptiquax.h>
#include <storage/queue.h>
#include "test.h"

Queue *queue = NULL;

int setup_queue() {
    printf("\n=== Setting up queue for tests ===");
    queue = malloc(sizeof(Queue));
    queue->next = NULL;
    return 0;
}

int teardown_queue() {
    printf("\n=== Tearing down queue after tests ===\n");
    queue_free(queue);
    free(queue);
    return 0;
}

void test_queue_push_and_pop() {
    int data1 = 10, data2 = 20, data3 = 30;

    // Push elements onto the queue
    queue_push(queue, &data1);
    queue_push(queue, &data2);
    queue_push(queue, &data3);

    // Pop elements and verify order
    CU_ASSERT(*(int *)queue_pop(queue) == 10);
    CU_ASSERT(*(int *)queue_pop(queue) == 20);
    CU_ASSERT(*(int *)queue_pop(queue) == 30);

    // Queue should now be empty
    CU_ASSERT(queue_is_empty(queue));
}

void test_queue_is_empty() {
    // Queue should initially be empty
    CU_ASSERT(queue_is_empty(queue));

    int data = 42;
    queue_push(queue, &data);

    // Queue should no longer be empty
    CU_ASSERT(!queue_is_empty(queue));

    queue_pop(queue);

    // Queue should be empty again
    CU_ASSERT(queue_is_empty(queue));
}

int test() {
    CU_pSuite pSuite = NULL;

    pSuite = CU_add_suite("Queue", setup_queue, teardown_queue);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
        (NULL == CU_add_test(pSuite, "Push and Pop elements", test_queue_push_and_pop)) ||
        (NULL == CU_add_test(pSuite, "Check if queue is empty", test_queue_is_empty))
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