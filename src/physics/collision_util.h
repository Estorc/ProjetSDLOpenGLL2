#pragma once

/**
 * @defgroup CollisionUtil Collision Utilities
 * @brief Utilities for handling collision detection and response in the physics engine.
 * 
 * This file contains functions and definitions that assist in detecting and responding to collisions
 * between objects in the physics simulation. It provides essential utilities to ensure accurate and 
 * efficient collision handling.
 * 
 * @{
 */


/**
 * @brief Projects a box onto a given axis.
 * 
 * @param axis The axis to project onto.
 * @param halfExtents The half extents of the box.
 * @param rotation The rotation matrix of the box.
 * @param boxCenter The center of the box.
 * @param minProj Pointer to store the minimum projection value.
 * @param maxProj Pointer to store the maximum projection value.
 */
void project_box_onto_axis(const vec3 axis, const vec3 halfExtents, const mat3 rotation, vec3 boxCenter, float *minProj, float *maxProj);

/**
 * @brief Projects a capsule onto a given axis.
 * 
 * @param axis The axis to project onto.
 * @param capsuleStart The start point of the capsule.
 * @param capsuleEnd The end point of the capsule.
 * @param capsuleRadius The radius of the capsule.
 * @param minProj Pointer to store the minimum projection value.
 * @param maxProj Pointer to store the maximum projection value.
 */
void project_capsule_onto_axis(vec3 axis, vec3 capsuleStart, vec3 capsuleEnd, float capsuleRadius, float *minProj, float *maxProj);

/**
 * @brief Finds the closest point on a triangle to a given point.
 * 
 * @param p The point to find the closest point to.
 * @param a The first vertex of the triangle.
 * @param b The second vertex of the triangle.
 * @param c The third vertex of the triangle.
 * @param closestPoint Pointer to store the closest point on the triangle.
 */
void closest_point_on_triangle(vec3 p, vec3 a, vec3 b, vec3 c, vec3 closestPoint);

/**
 * @brief Checks if two projections overlap.
 * 
 * @param minA The minimum value of the first projection.
 * @param maxA The maximum value of the first projection.
 * @param minB The minimum value of the second projection.
 * @param maxB The maximum value of the second projection.
 * @return true if the projections overlap, false otherwise.
 */
bool projections_overlap(float minA, float maxA, float minB, float maxB);

/**
 * @brief Computes the overlap between two projections.
 * 
 * @param minA The minimum value of the first projection.
 * @param maxA The maximum value of the first projection.
 * @param minB The minimum value of the second projection.
 * @param maxB The maximum value of the second projection.
 * @return The amount of overlap between the two projections.
 */
float compute_overlap(float minA, float maxA, float minB, float maxB);

/** @} */