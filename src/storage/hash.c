#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hash.h"

#define LOAD_FACTOR 0.75f
#define MULTIPLIER 31

unsigned int hash(const char *key, int capacity) {
    unsigned int hash = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hash = MULTIPLIER * hash + key[i];
    }
    return hash % capacity;
}

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

Entry * table_insert(HashTable *table, const char *key, void *value) {
    if (table->size >= table->capacity * LOAD_FACTOR) {
        table_resize(table);
    }

    unsigned int index = hash(key, table->capacity);
    Entry *entry = table->table[index];
    while (entry) {
        if (strcmp(entry->key, key) == 0) {
            if (entry->needs_free) free(entry->value);
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