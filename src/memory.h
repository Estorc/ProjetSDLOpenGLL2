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


typedef struct ModelData ModelData;
typedef u32 TextureMap;
typedef unsigned int Shader;

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
 * @brief Structure to cache music.
 * 
 * This structure holds a music file and its associated name.
 */
typedef struct {
    Mix_Music *music;
    char musicName[100];
} MixMusicCache;

/**
 * @brief Structure to cache sound effects.
 * 
 * This structure holds a sound effect and its associated name.
 */
typedef struct {
    Mix_Chunk *chunk;
    char chunkName[100];
} MixChunkCache;

/**
 * @brief Structure to manage all types of caches.
 * 
 * This structure holds caches for cube maps, textures, models, and shaders,
 * along with their respective counts.
 */
typedef struct MemoryCaches {
    CubeMapCache *cubeMapCache; /**< Pointer to the array of cube map caches. */
    int cubeMapCount; /**< Number of cube map caches. */
    TextureCache *textureCache; /**< Pointer to the array of texture caches. */
    int texturesCount; /**< Number of texture caches. */
    ModelCache *modelCache; /**< Pointer to the array of model caches. */
    int modelsCount; /**< Number of model caches. */
    ShaderCache *shaderCache; /**< Pointer to the array of shader caches. */
    int shadersCount; /**< Number of shader caches. */
    MixMusicCache *musicCache; /**< Pointer to the array of music caches. */
    int musicCount; /**< Number of music caches. */
    MixChunkCache *chunkCache; /**< Pointer to the array of sound effect caches. */
    int chunkCount; /**< Number of sound effect caches. */

} MemoryCaches;

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
 * @brief Gets a model from the cache.
 * 
 * @param name The name of the model.
 * @return A pointer to the model data.
 * 
 * This function retrieves a model from the cache with the specified name.
 * If the model is not found, it returns NULL.
 */
ModelData * get_model_from_cache(const char *name);

/**
 * @brief Adds a model to the cache.
 * 
 * @param name The name of the model.
 * @param model The model data to add to the cache.
 * 
 * This function adds a model to the cache with the specified name.
 */
void add_model_to_cache(const char *name, ModelData *model);

/**
 * @brief Frees the allocated models.
 * 
 * This function releases the memory allocated for models.
 */
void free_models();

/**
 * @brief Gets a music from the cache.
 * 
 * @param name The name of the music.
 * @return A pointer to the music data.
 * 
 * This function retrieves a music from the cache with the specified name.
 */
Mix_Music * get_mix_music_from_cache(const char *name);

/**
 * @brief Adds a music to the cache.
 * 
 * @param name The name of the music.
 * @param music The music data to add to the cache.
 * 
 * This function adds a music to the cache with the specified name.
 */
void add_mix_music_to_cache(const char *name, Mix_Music *music);

/**
 * @brief Frees the allocated music.
 * 
 * This function releases the memory allocated for music.
 */
void free_musics();

/**
 * @brief Gets a sound effect from the cache.
 * 
 * @param name The name of the sound effect.
 * @return A pointer to the sound effect data.
 * 
 * This function retrieves a sound effect from the cache with the specified name.
 */
Mix_Chunk * get_mix_chunk_from_cache(const char *name);

/**
 * @brief Adds a sound effect to the cache.
 * 
 * @param name The name of the sound effect.
 * @param music The sound effect data to add to the cache.
 * 
 * This function adds a sound effect to the cache with the specified name.
 */
void add_mix_chunk_to_cache(const char *name, Mix_Chunk *chunk);

/**
 * @brief Frees the allocated sound effects.
 * 
 * This function releases the memory allocated for sound effects.
 */
void free_chunks();

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