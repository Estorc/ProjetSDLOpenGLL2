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
