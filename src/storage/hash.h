#ifndef HASH_H
#define HASH_H

#include <stdbool.h>

/**
 * @file hash.h
 * @brief Header file for a simple hash table implementation.
 *
 * This file contains the definitions and function declarations for a hash table
 * implementation that supports insertion, retrieval, and deletion of key-value pairs.
 */

/**
 * @struct Entry
 * @brief Represents an entry in the hash table.
 *
 * @var Entry::key
 * The key associated with this entry.
 *
 * @var Entry::value
 * The value associated with this entry.
 *
 * @var Entry::needs_free
 * A flag indicating whether the value needs to be freed when the entry is removed.
 *
 * @var Entry::next
 * A pointer to the next entry in the same bucket (for handling collisions).
 */
typedef struct Entry {
    char *key;
    void *value;
    bool needs_free;
    struct Entry *next;
} Entry;

/**
 * @struct HashTable
 * @brief Represents the hash table.
 *
 * @var HashTable::table
 * An array of pointers to entries, representing the buckets of the hash table.
 *
 * @var HashTable::size
 * The current number of entries in the hash table.
 *
 * @var HashTable::capacity
 * The current capacity of the hash table (number of buckets).
 */
typedef struct HashTable {
    Entry **table;
    int size;
    int capacity;
} HashTable;

/**
 * @brief Computes the hash value for a given key.
 *
 * @param key The key to hash.
 * @param capacity The capacity of the hash table.
 * @return The hash value for the key.
 */
unsigned int hash(const char *key, int capacity);

/**
 * @brief Creates a new hash table with the specified initial capacity.
 *
 * @param initial_capacity The initial capacity of the hash table.
 * @return A pointer to the newly created hash table.
 */
HashTable *table_create(int initial_capacity);

/**
 * @brief Resizes the hash table to accommodate more entries.
 *
 * @param table The hash table to resize.
 */
void table_resize(HashTable *table);

/**
 * @brief Inserts a key-value pair into the hash table.
 *
 * @param table The hash table.
 * @param key The key to insert.
 * @param value The value to associate with the key.
 * @return A pointer to the newly inserted entry.
 */
Entry *table_insert(HashTable *table, const char *key, void *value);

/**
 * @brief Inserts a key-value pair into the hash table without checking for duplicates.
 *
 * @param table The hash table.
 * @param key The key to insert.
 * @param value The value to associate with the key.
 */
void table_insert_raw(HashTable *table, const char *key, void *value);

/**
 * @brief Retrieves the value associated with a given key in the hash table.
 *
 * @param table The hash table.
 * @param key The key to look up.
 * @return A pointer to the value associated with the key, or NULL if the key is not found.
 */
void *table_get(HashTable *table, const char *key);

/**
 * @brief Removes a key-value pair from the hash table.
 *
 * @param table The hash table.
 * @param key The key to remove.
 */
void table_remove(HashTable *table, const char *key);

/**
 * @brief Frees all memory associated with the hash table.
 *
 * @param table The hash table to free.
 */
void table_free(HashTable *table);

#endif