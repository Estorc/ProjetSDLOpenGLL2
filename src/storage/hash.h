#ifndef HASH_H
#define HASH_H

#include <stdbool.h>

// Define the Entry struct
typedef struct Entry {
    char *key;
    void *value;
    bool needs_free;
    struct Entry *next;
} Entry;

// Define the HashTable struct
typedef struct HashTable {
    Entry **table;
    int size;
    int capacity;
} HashTable;

// Function declarations
unsigned int hash(const char *key, int capacity);
HashTable *table_create(int initial_capacity);
void table_resize(HashTable *table);
Entry *table_insert(HashTable *table, const char *key, void *value);
void table_insert_raw(HashTable *table, const char *key, void *value);
void *table_get(HashTable *table, const char *key);
void table_remove(HashTable *table, const char *key);
void table_free(HashTable *table);

#endif // HASH_H
