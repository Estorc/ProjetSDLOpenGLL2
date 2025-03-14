#include "../src/raptiquax.h"
#include "../src/storage/hash.h"
#include "../src/storage/raw.h"
#include <assert.h>

int main(int argc, char **argv) {
    // Create the hash table with an initial capacity
    HashTable* table = table_create(16);

    // Insert some key-value pairs
    table_insert_raw(table, "apple", NEW_RAW(int, 1));
    table_insert_raw(table, "banana", NEW_RAW(int, 2));
    table_insert_raw(table, "cherry", NEW_RAW(int, 3));

    // Find values by key
    assert(GET_RAW(int, table_get(table, "apple")) == 1);
    assert(GET_RAW(int, table_get(table, "banana")) == 2);
    assert(GET_RAW(int, table_get(table, "cherry")) == 3);
    assert(GET_RAW(int, table_get(table, "grape")) == 0);  // Should return 0 (not found)

    // Delete an entry
    table_remove(table, "banana");
    assert(GET_RAW(int, table_get(table, "banana")) == 0);  // Should return 0

    // Free memory used by the hash table
    table_free(table);

    printf("All tests passed!\n");

    return 0;
}