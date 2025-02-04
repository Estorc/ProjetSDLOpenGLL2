#ifndef MEMORY_H
#define MEMORY_H

/**
 * @file memory.h
 * @brief Header file for memory management functions and structures.
 * 
 * This file contains declarations for functions and structures used to manage
 * memory caches, shaders, models, textures, and cubemaps in the project.
 */

/**
 * @defgroup MemoryCache Memory Cache Management
 * @brief Functions and structures for managing memory caches.
 * 
 * This module provides structures and functions to manage caches for cube maps,
 * textures, models, and shaders. It includes initialization and cleanup functions
 * to handle the memory allocation and deallocation for these resources.
 * 
 * @{
 */

/**
 * @brief Structure to cache cube map textures.
 * 
 * This structure holds a cube map texture and its associated names.
 */
typedef struct {
    TextureMap cubeMap; /**< The cube map texture. */
    char textureName[6][100]; /**< Names of the textures for each face of the cube map. */
} CubeMapCache;

/**
 * @brief Structure to cache textures.
 * 
 * This structure holds a texture and its associated name.
 */
typedef struct {
    TextureMap textureMap; /**< The texture. */
    char textureName[100]; /**< Name of the texture. */
} TextureCache;

/**
 * @brief Structure to cache models.
 * 
 * This structure holds a model and its associated name.
 */
typedef struct {
    ModelData *model; /**< Pointer to the model data. */
    char modelName[100]; /**< Name of the model. */
} ModelCache;

/**
 * @brief Structure to cache shaders.
 * 
 * This structure holds a shader and its associated names.
 */
typedef struct {
    Shader shader; /**< The shader. */
    char shaderName[2][100]; /**< Names of the shaders. */
} ShaderCache;

/**
 * @brief Structure to manage all types of caches.
 * 
 * This structure holds caches for cube maps, textures, models, and shaders,
 * along with their respective counts.
 */
typedef struct {
    CubeMapCache *cubeMapCache; /**< Pointer to the array of cube map caches. */
    int cubeMapCount; /**< Number of cube map caches. */
    TextureCache *textureCache; /**< Pointer to the array of texture caches. */
    int texturesCount; /**< Number of texture caches. */
    ModelCache *modelCache; /**< Pointer to the array of model caches. */
    int modelsCount; /**< Number of model caches. */
    ShaderCache *shaderCache; /**< Pointer to the array of shader caches. */
    int shadersCount; /**< Number of shader caches. */
} MemoryCaches;

/**
 * @brief External variable representing the memory caches.
 */
extern MemoryCaches memoryCaches;

/**
 * @brief Initializes the memory cache.
 * 
 * This function sets up the memory cache for use in the project.
 */
void init_memory_cache();

/**
 * @brief Frees the allocated shaders.
 * 
 * This function releases the memory allocated for shaders.
 */
void free_shaders();

/**
 * @brief Frees the allocated models.
 * 
 * This function releases the memory allocated for models.
 */
void free_models();

/**
 * @brief Frees the allocated textures.
 * 
 * This function releases the memory allocated for textures.
 */
void free_textures();

/**
 * @brief Frees the allocated cubemaps.
 * 
 * This function releases the memory allocated for cubemaps.
 */
void free_cubemaps();

/**
 * @brief Frees the entire memory cache.
 * 
 * This function releases all memory allocated in the memory cache.
 */
void free_memory_cache();

/** @} */ // end of MemoryCache group
#endif