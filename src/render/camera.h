#include "../types.h"
#include "../math/math_util.h"
#include "../io/shader.h"

#ifndef CAMERA_H
#define CAMERA_H

struct Node;
struct WorldShaders;

/**
 * @defgroup Camera Camera Functions
 * @brief Functions for initializing and managing the camera.
 * @{
 */

/**
 * @struct Camera
 * @brief A structure to represent a camera with position and rotation in 3D space.
 */

typedef struct Camera {
    vec3 pos; /**< Position of the camera. */
    vec3 rot; /**< Rotation of the camera. */
} Camera;

/**
 * @brief Initializes the camera.
 * 
 * This function sets up the initial state of the camera, including its position,
 * orientation, and any other relevant parameters. It must be called before using
 * the camera in rendering operations.
 * 
 * @param c Pointer to a Camera structure that will be initialized.
 */
void init_camera(Camera *c);

/**
 * @brief Sets up the camera projection.
 * 
 * This function configures the camera's projection matrix and applies it to the
 * provided shaders. It ensures that the rendered scene is correctly projected
 * from the camera's point of view.
 * 
 * @param c Pointer to a Camera structure that contains the camera's parameters.
 * @param shaders Pointer to a WorldShaders structure that will be updated with the
 *                camera's projection matrix.
 */
void camera_projection(Camera *c, struct WorldShaders *shaders);

/** @} */ // end of Camera group

#endif