#pragma once

#ifdef _WIN32
#include <GL/glew.h>  // Include before OpenGL functions
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <limits.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <cglm/cglm.h>
#include <math.h>
#include "io/stringio.h"
#include "config.h"

/**
 * @file raptiquax.h
 * @brief This file contains type definitions for fixed-width integer types and floating-point types.
 */


struct MemoryCaches;
struct BufferCollection;
struct Queue;
struct Tree;
struct Input;
struct Settings;
struct Window;
struct Camera;
struct RenderTarget;
struct Script;
struct ClassManager;
struct HashTable;

/**
 * @brief The main structure that holds all the data and state of the engine.
 * 
 * This structure holds all the data and state of the engine, including memory caches,
 * buffers, the call queue, the main node tree, input data, settings, and the window.
 * 
 * @note This structure is used as a singleton to hold the global state of the engine.
 * It use pointers to the actual data structures to avoid including too much headers in there.
 */
struct RaptiquaX_t {
    struct MemoryCaches * const memoryCaches;  /**< The memory caches of the engine. */
    struct BufferCollection * const buffers;  /**< The buffer collection of the engine. */
    struct Queue * const callQueue;  /**< The call queue of the engine. */
    struct Tree * const mainTree;  /**< The main node tree of the engine. */
    struct Input * const input;  /**< The input data of the engine. */
    struct Settings * const settings;  /**< The settings of the engine. */
    struct Window * const window;  /**< The window of the engine. */
    const struct ClassManager * const classManager;  /**< The class manager of the engine. */
    struct Camera *camera; /**< The camera actually used. */
    struct MSAA *msaa; /**< The MSAA settings. */
    struct RenderTarget *renderTarget; /**< The actual render target. */
    struct HashTable *storage; /**< Global storage in a Hash Table */
    struct Script *scripts; /**< The scripts. */
};

 extern struct RaptiquaX_t Game;


/**
 * @def u8
 * @brief Alias for uint8_t (unsigned 8-bit integer).
 */
#define u8 uint8_t

/**
 * @def u16
 * @brief Alias for uint16_t (unsigned 16-bit integer).
 */
#define u16 uint16_t

/**
 * @def u32
 * @brief Alias for uint32_t (unsigned 32-bit integer).
 */
#define u32 uint32_t

/**
 * @def u64
 * @brief Alias for uint64_t (unsigned 64-bit integer).
 */
#define u64 uint64_t

/**
 * @def s8
 * @brief Alias for int8_t (signed 8-bit integer).
 */
#define s8 int8_t

/**
 * @def s16
 * @brief Alias for int16_t (signed 16-bit integer).
 */
#define s16 int16_t

/**
 * @def s32
 * @brief Alias for int32_t (signed 32-bit integer).
 */
#define s32 int32_t

/**
 * @def s64
 * @brief Alias for int64_t (signed 64-bit integer).
 */
#define s64 int64_t

/**
 * @def f32
 * @brief Alias for float (32-bit floating-point).
 */
#define f32 float

/**
 * @def f64
 * @brief Alias for double (64-bit floating-point).
 */
#define f64 double
