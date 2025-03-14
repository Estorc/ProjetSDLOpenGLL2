#include "../io/model.h"
#include "framebuffer.h"
#ifndef DEPTH_MAP_H
#define DEPTH_MAP_H

#define NUM_DIRECTIONAL_LIGHTS 10
#define NUM_POINT_LIGHTS 10
#define NUM_SPOT_LIGHTS 10

struct WorldShaders;


/**
 * @defgroup DepthMap Depth Map
 * @brief Functions related to depth map creation and management.
 * @{
 */

/**
 * @struct DepthMap
 * @brief Represents a depth map used in rendering.
 *
 * This structure encapsulates the necessary components for a depth map,
 * including a frame buffer object (FBO), a texture map, and a texture buffer object (TBO).
 */
typedef struct DepthMap {
    FBO frameBuffer[NUM_DIRECTIONAL_LIGHTS + NUM_POINT_LIGHTS * 6 + NUM_SPOT_LIGHTS]; /**< The frame buffer object for the depth map. */
    TextureMap texture; /**< The texture map for the depth map. */
    GLuint tbo; /**< The texture buffer object for the depth map. */
    TextureMap matrixTexture; /**< The texture map for the light matrices. */
} DepthMap;

/**
 * @brief Creates a depth map for shadow mapping.
 *
 * This function initializes and configures a depth map, which is used for shadow mapping in a 3D scene.
 * It sets up the necessary framebuffer, texture, and other OpenGL resources required for rendering
 * depth information from the light's perspective.
 *
 * @param depthMap A pointer to a DepthMap structure that will be initialized.
 * @param shaders A pointer to a WorldShaders structure containing the shaders used for rendering the depth map.
 *
 * @note Ensure that the DepthMap and WorldShaders structures are properly allocated before calling this function.
 *       This function should be called during the initialization phase of your rendering setup.
 *
 * @see DepthMap
 * @see WorldShaders
 */
void create_depthmap(DepthMap *depthMap, struct WorldShaders *shaders);

/** @} */
#endif