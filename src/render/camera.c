#include "camera.h"
#include "render.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../io/shader.h"
#include "../memory.h"
#include "../storage/node.h"
#include <SDL2/SDL.h>
#include "../window.h"


/**
 * Initializes a Camera structure by setting its position, direction,
 * rotation, target rotation, and target position to zero.
 * 
 * @param c {Camera*} - Pointer to the Camera structure to be initialized.
 * 
 * This function is typically called when creating a new camera instance 
 * to ensure all fields are set to a known starting state. This is 
 * particularly important in graphics applications where camera parameters 
 * can influence rendering and scene traversal.
 */

void init_camera(Camera *c) {

    Vec3fZero(c->pos);
    Vec3fZero(c->rot);

}


/**
 * Updates the view and projection matrices based on the camera's current position and direction,
 * and sends these matrices to the appropriate shaders for rendering.
 * 
 * @param c {Camera*} - Pointer to the Camera structure containing the camera's position and direction.
 * @param shaders {shaders[]} - Array of shaders to be used for rendering the scene.
 */

void camera_projection(Camera *c, WorldShaders *shaders) {
    int window_width, window_height;
    get_resolution(&window_width, &window_height);
    // Camera
    mat4 view = GLM_MAT4_IDENTITY_INIT;

    glm_rotate(view, to_radians(c->rot[0]), (vec3){1.0f, 0.0f, 0.0f});
    glm_rotate(view, to_radians(c->rot[1]), (vec3){0.0f, 1.0f, 0.0f});
    glm_rotate(view, to_radians(c->rot[2]), (vec3){0.0f, 0.0f, 1.0f});
    glm_translate(view, (vec3){c->pos[0], c->pos[1], c->pos[2]});

    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_perspective(PI/4, (float)window_width/(float)window_height, 0.1f, 300.0f, projection);

    vec3 viewPos;
    glm_vec3_negate_to(c->pos, viewPos);

    for (int i = 0; i < memoryCaches.shadersCount; i++) {
        use_shader(memoryCaches.shaderCache[i].shader);
        glUniformMatrix4fv(glGetUniformLocation(memoryCaches.shaderCache[i].shader, "projection"), 1, GL_FALSE, &projection);
        glUniformMatrix4fv(glGetUniformLocation(memoryCaches.shaderCache[i].shader, "view"), 1, GL_FALSE, &view);
        glUniform3fv(glGetUniformLocation(memoryCaches.shaderCache[i].shader, "viewPos"), 1, &viewPos);
    }
}