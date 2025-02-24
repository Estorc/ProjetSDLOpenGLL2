#ifndef BUFFER_H
#define BUFFER_H

/**
 * @file buffer.h
 * @brief Functions for initializing and freeing Game.buffers->
 * 
 * This file contains the declarations of functions used to initialize
 * and free buffers in the project.
 */

/**
 * @defgroup BufferManagement Buffer Management
 * @brief Functions for managing Game.buffers->
 * @{
 */

struct LightingBuffer;
struct CollisionBuffer;

/**
 * @brief Structure to hold various buffer collections.
 * 
 * This structure contains buffers for lighting and collision data.
 */
typedef struct BufferCollection {
    struct LightingBuffer lightingBuffer; /**< Buffer for lighting data. */
    struct CollisionBuffer collisionBuffer; /**< Buffer for collision data. */
} BufferCollection;

/**
 * @brief Initializes the Game.buffers->
 * 
 * This function sets up the necessary buffers for the application.
 * It should be called before any buffer operations are performed.
 */
void init_buffers();

/**
 * @brief Frees the Game.buffers->
 * 
 * This function releases the resources allocated for the Game.buffers->
 * It should be called when buffer operations are no longer needed.
 */
void free_buffers();

/** @} */ // end of BufferManagement

#endif