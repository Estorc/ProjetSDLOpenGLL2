#pragma once

#include <raptiquax/std.h>
#include <raptiquax/GL.h>
#include <raptiquax/SDL.h>
#include <raptiquax/aliases.h>
#include <raptiquax/structs.h>
#include <io/stringio.h>
#include <config.h>

/**
 * @file raptiquax.h
 * @brief This file contains type definitions for fixed-width integer types and floating-point types.
 */

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
    struct DFBO *deferredBuffer; /**< The SSR settings. */
    struct CFBO *uiFBO; /**< The UI frame buffer object. */
    struct RenderTarget *renderTarget; /**< The actual render target. */
    struct HashTable *storage; /**< Global storage in a Hash Table */
    struct Script *scripts; /**< The scripts. */
    struct DepthMap *depthMap; /**< The depth map. */
};

extern struct RaptiquaX_t Game;