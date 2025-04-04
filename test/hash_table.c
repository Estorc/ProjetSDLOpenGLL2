#include <raptiquax.h>
#include <storage/hash.h>
#include <storage/raw.h>
#include "test.h"


HashTable* table = NULL;

int setup_hash_table() {
    printf("\n=== Setting up hash table for tests ===");
    table = table_create(16);
    return 0;
}
int teardown_hash_table() {
    printf("\n=== Tearing down hash table after tests ===\n");
    table_free(table);
    return 0;
}

void test_table_get() {
    // Insert some key-value pairs
    table_insert_raw(table, "apple", NEW_RAW(int, 1));
    table_insert_raw(table, "banana", NEW_RAW(int, 2));
    table_insert_raw(table, "cherry", NEW_RAW(int, 3));

    // Find values by key
    CU_ASSERT(GET_RAW(int, table_get(table, "apple")) == 1);
    CU_ASSERT(GET_RAW(int, table_get(table, "banana")) == 2);
    CU_ASSERT(GET_RAW(int, table_get(table, "cherry")) == 3);
    CU_ASSERT(GET_RAW(int, table_get(table, "grape")) == 0);  // Should return 0 (not found)
}

void test_table_remove() {
    // Remove an entry
    table_remove(table, "banana");
    CU_ASSERT(GET_RAW(int, table_get(table, "banana")) == 0);  // Should return 0
}

int test() {
    CU_pSuite pSuite = NULL;

    pSuite = CU_add_suite("Hash Table", setup_hash_table, teardown_hash_table);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
        (NULL == CU_add_test(pSuite, "Retrieve \"raw\" element", test_table_get)) ||
        (NULL == CU_add_test(pSuite, "Trying retrieving a deleted element", test_table_remove))
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