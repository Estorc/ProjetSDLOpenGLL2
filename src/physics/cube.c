#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "../types.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
#include "../window.h"
#include "../render/color.h"
#include "../render/camera.h"
#include "../render/depth_map.h"
#include "../render/lighting.h"
#include "../io/gltexture_loader.h"
#include "physics.h"
#include "bodies.h"

// Function to project a box onto an axis and return min and max projection values
void project_box_onto_axis(vec3 axis, vec3 halfExtents, mat3 rotation, vec3 boxCenter, float *minProj, float *maxProj) {
    // Project the center of the box onto the axis
    float centerProjection = glm_vec3_dot(boxCenter, axis);
    
    // Calculate the extent of the box in the direction of the axis
    float extentProjection = fabs(glm_vec3_dot(rotation[0], axis) * halfExtents[0]) +
                             fabs(glm_vec3_dot(rotation[1], axis) * halfExtents[1]) +
                             fabs(glm_vec3_dot(rotation[2], axis) * halfExtents[2]);
                             
    // Set the min and max projection values
    *minProj = centerProjection - extentProjection;
    *maxProj = centerProjection + extentProjection;
}

// Check if two projection ranges overlap
bool projections_overlap(float minA, float maxA, float minB, float maxB) {
    return (minA <= maxB && maxA >= minB);
}

// Compute the overlap between two projections
float compute_overlap(float minA, float maxA, float minB, float maxB) {
    return fminf(maxA, maxB) - fmaxf(minA, minB);
}