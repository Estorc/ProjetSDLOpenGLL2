#pragma once

/**
 * @file deferred.h
 * @brief Header file for deferred rendering utilities.
 *
 * This file contains function declarations for initializing and managing deferred rendering,
 * including setting up the framebuffer, textures, and shaders.
 */

/**
 * @brief Call in deferred the function "table_get" to get a value from the table.
 * 
 * This function retrieves a value from the deferred table using the provided key.
 * It is typically used to access data stored in the table for rendering or processing.
 * 
 * @param table Pointer to the HashTable structure representing the deferred table.
 * @param key The key used to look up the value in the table.
 * @param out Pointer to the location where the retrieved value will be stored.
 */
void deferred_table_get(struct HashTable * table, char * key, void * out);

/**
 * @brief Call in deferred the function "load_scene" to load a new scene.
 * 
 * This function prepares the scene for loading by pushing the necessary data to the call queue.
 * It is typically used to change the current scene in the game or application.
 * 
 * @param path The path to the new scene to be loaded.
 */
void deferred_load_scene(char *path);