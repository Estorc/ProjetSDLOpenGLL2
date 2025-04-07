#pragma once

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

/**
 * @typedef TextureMap
 * @brief Alias for u32 representing a texture map.
 */
typedef u32 TextureMap;

/**
 * @typedef VBO
 * @brief Alias for unsigned int representing a Vertex Buffer Object.
 */
typedef u32 VBO;

/**
 * @typedef VAO
 * @brief Alias for unsigned int representing a Vertex Array Object.
 */
typedef u32 VAO;

/**
 * @typedef VertexBuffer
 * @brief Alias for VBO.
 */
typedef VBO VertexBuffer;

/**
 * @typedef VertexArray
 * @brief Alias for VAO.
 */
typedef VAO VertexArray;


/**
 * @typedef Normal
 * @brief Alias for vec3 representing a normal vector.
 */
typedef vec3 Normal;

/**
 * @typedef TextureVertex
 * @brief Alias for vec2 representing a texture vertex.
 */
typedef vec2 TextureVertex;

/**
 * @typedef FBO
 * @brief Alias for an unsigned int representing a Frame Buffer Object.
 */
typedef u32 FBO;

/**
 * @typedef FrameBuffer
 * @brief Alias for FBO, representing a Frame Buffer Object.
 */
typedef FBO FrameBuffer;

/**
 * @typedef RBO
 * @brief Alias for an unsigned int representing a Render Buffer Object.
 */
typedef u32 RBO;

/**
 * @typedef Shader
 * @brief Represents an unsigned integer used as a shader identifier.
 */
typedef u32 Shader;