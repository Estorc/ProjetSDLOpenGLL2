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

void project_capsule_onto_axis(vec3 axis, vec3 capsuleStart, vec3 capsuleEnd, float capsuleRadius, float *minProj, float *maxProj) {
    // Project the start and end points of the capsule onto the axis
    float startProjection = glm_vec3_dot(capsuleStart, axis);
    float endProjection = glm_vec3_dot(capsuleEnd, axis);
    
    // Determine the min and max projections including the radius
    *minProj = fminf(startProjection, endProjection) - capsuleRadius;
    *maxProj = fmaxf(startProjection, endProjection) + capsuleRadius;
}

// Check if two projection ranges overlap
bool projections_overlap(float minA, float maxA, float minB, float maxB) {
    return (minA <= maxB && maxA >= minB);
}

// Compute the overlap between two projections
float compute_overlap(float minA, float maxA, float minB, float maxB) {
    return fminf(maxA, maxB) - fmaxf(minA, minB);
}



void closest_point_on_triangle(vec3 p, vec3 a, vec3 b, vec3 c, vec3 closestPoint) {
    // Compute vectors
    vec3 ab, ac, ap;
    glm_vec3_sub(b, a, ab);
    glm_vec3_sub(c, a, ac);
    glm_vec3_sub(p, a, ap);

    // Compute dot products
    float d1 = glm_vec3_dot(ab, ap);
    float d2 = glm_vec3_dot(ac, ap);

    // Check if P in vertex region outside A
    if (d1 <= 0.0f && d2 <= 0.0f) {
        glm_vec3_copy(a, closestPoint);
        return;
    }

    // Check if P in vertex region outside B
    vec3 bp;
    glm_vec3_sub(p, b, bp);
    float d3 = glm_vec3_dot(ab, bp);
    float d4 = glm_vec3_dot(ac, bp);
    if (d3 >= 0.0f && d4 <= d3) {
        glm_vec3_copy(b, closestPoint);
        return;
    }

    // Check if P in edge region of AB, if so return projection of P onto AB
    float vc = d1 * d4 - d3 * d2;
    if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
        float v = d1 / (d1 - d3);
        glm_vec3_scale(ab, v, ab);
        glm_vec3_add(a, ab, closestPoint);
        return;
    }

    // Check if P in vertex region outside C
    vec3 cp;
    glm_vec3_sub(p, c, cp);
    float d5 = glm_vec3_dot(ab, cp);
    float d6 = glm_vec3_dot(ac, cp);
    if (d6 >= 0.0f && d5 <= d6) {
        glm_vec3_copy(c, closestPoint);
        return;
    }

    // Check if P in edge region of AC, if so return projection of P onto AC
    float vb = d5 * d2 - d1 * d6;
    if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
        float w = d2 / (d2 - d6);
        glm_vec3_scale(ac, w, ac);
        glm_vec3_add(a, ac, closestPoint);
        return;
    }

    // Check if P in edge region of BC, if so return projection of P onto BC
    float va = d3 * d6 - d5 * d4;
    if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
        float u = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        vec3 bc;
        glm_vec3_sub(c, b, bc);
        glm_vec3_scale(bc, u, bc);
        glm_vec3_add(b, bc, closestPoint);
        return;
    }

    // P inside face region. Compute Q through its barycentric coordinates (u, v, w)
    float denom = 1.0f / (va + vb + vc);
    float v = vb * denom;
    float w = vc * denom;
    glm_vec3_scale(ab, v, ab);
    glm_vec3_scale(ac, w, ac);
    glm_vec3_add(a, ab, closestPoint);
    glm_vec3_add(closestPoint, ac, closestPoint);

    // Ensure the closest point is on the correct side of the triangle
    vec3 normal;
    glm_vec3_cross(ab, ac, normal);
    glm_vec3_normalize(normal);
    vec3 toPoint;
    glm_vec3_sub(p, closestPoint, toPoint);
    if (glm_vec3_dot(normal, toPoint) < 0.0f) {
        glm_vec3_negate(normal);
    }
}