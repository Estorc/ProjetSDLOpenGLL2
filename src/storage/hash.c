#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hash.h"

#define LOAD_FACTOR 0.75f       // Threshold for resizing the table
#define MULTIPLIER 31           // Multiplier for the hash function

// Hash function
unsigned int hash(const char *key, int capacity) {
    unsigned int hash = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hash = MULTIPLIER * hash + key[i];
    }
    return hash % capacity;
}

// Create a new hash table
HashTable *table_create(int initial_capacity) {
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));
    table->table = (Entry **)calloc(initial_capacity, sizeof(Entry *));
    table->size = 0;
    table->capacity = initial_capacity;
    return table;
}

void table_resize(HashTable *table) {
    int new_capacity = table->capacity << 1;
    Entry **new_table = (Entry **)calloc(new_capacity, sizeof(Entry *));

    for (int i = 0; i < table->capacity; i++) {
        Entry *entry = table->table[i];
        while (entry) {
            unsigned int index = hash(entry->key, new_capacity);
            Entry *next = entry->next;
            entry->next = new_table[index];
            new_table[index] = entry;
            entry = next;
        }
    }

    free(table->table);
    table->table = new_table;
    table->capacity = new_capacity;
}

// Insert a key-value pair into the hash table
Entry * table_insert(HashTable *table, const char *key, void *value) {
    if (table->size >= table->capacity * LOAD_FACTOR) {
        table_resize(table);
    }

    unsigned int index = hash(key, table->capacity);
    Entry *entry = table->table[index];
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            entry->needs_free = true;
            free(entry->value);
            entry->value = value;
            return entry;
        }
        entry = entry->next;
    }

    Entry *new_entry = (Entry *)malloc(sizeof(Entry));
    new_entry->key = strdup(key);
    new_entry->value = value;
    new_entry->next = table->table[index];
    new_entry->needs_free = false;
    table->table[index] = new_entry;
    table->size++;
    return new_entry;
}

void table_insert_raw(HashTable *table, const char *key, void *value) {
    table_insert(table, key, value)->needs_free = true;
}


// Retrieve a value from the hash table
void *table_get(HashTable *table, const char *key) {
    unsigned int index = hash(key, table->capacity);
    Entry *entry = table->table[index];
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

// Remove a key-value pair from the hash table
void table_remove(HashTable *table, const char *key) {
    unsigned int index = hash(key, table->capacity);
    Entry *entry = table->table[index];
    Entry *prev = NULL;
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            if (prev) {
                prev->next = entry->next;
            } else {
                table->table[index] = entry->next;
            }
            if (entry->needs_free) {
                free(entry->value);
            }
            free(entry->key);
            free(entry);
            table->size--;
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}

// Free the memory used by the hash table
void table_free(HashTable *table) {
    for (int i = 0; i < table->capacity; i++) {
        Entry *entry = table->table[i];
        while (entry) {
            Entry *next = entry->next;
            free(entry->key);
            if (entry->needs_free) {
                free(entry->value);
            }
            free(entry);
            entry = next;
        }
    }
    free(table->table);
    free(table);
}