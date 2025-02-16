#include "../raptiquax.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
#include "../window.h"
#include "../render/camera.h"
#include "../render/depth_map.h"
#include "../render/lighting.h"
#include "../io/gltexture_loader.h"
#include "../classes/classes.h"
#include "../memory.h"
#include "../buffer.h"
#include "physics.h"
#include "bodies.h"
#include "collision.h"
#include "collision_util.h"



unsigned int get_collision_code(Node *shapeA, Node *shapeB) {
    int priorityA, priorityB;
    shapeA::get_priority(&priorityA);
    shapeB::get_priority(&priorityB);
    if (priorityA < priorityB)
        return shapeA->type | shapeB->type << 8;
    return shapeB->type | shapeA->type << 8;
}


void apply_collision(Node *shapeA, Node *shapeB, vec3 collisionNormal, vec3 impactPoint, float penetrationDepth) {
    bool conditionA, conditionB;
    (shapeA->parent)::is_body(&conditionA);
    (shapeB->parent)::is_body(&conditionB);
    if (conditionA && conditionB)
        apply_body_collision(shapeA, shapeB, collisionNormal, impactPoint, penetrationDepth);
    (shapeA->parent)::is_area(&conditionA);
    if (conditionA)
        (shapeA->parent)::collect_node(shapeB->parent, penetrationDepth, impactPoint);
    (shapeB->parent)::is_area(&conditionB);
    if (conditionB)
        (shapeB->parent)::collect_node(shapeA->parent, penetrationDepth, impactPoint);
    
}



// Main SAT algorithm for box-box collision detection
bool check_collision_box_with_box(Node *boxB, Node *boxA) {
    vec3 collisionNormal;
    vec3 impactPoint;
    float penetrationDepth;

    // Check if rotated and adapt to
    if (
        !fmod(boxB->globalRot[0],90.0) &&
        !fmod(boxB->globalRot[1],90.0) &&
        !fmod(boxB->globalRot[2],90.0) &&
        !fmod(boxA->globalRot[0],90.0) &&
        !fmod(boxA->globalRot[1],90.0) &&
        !fmod(boxA->globalRot[2],90.0)
    ) {

        // Get cube properties
        vec3 halfExtentsA, halfExtentsB;
        glm_vec3_copy(boxB->globalScale, halfExtentsA);
        glm_vec3_copy(boxA->globalScale, halfExtentsB);

        // Step 1: Transform the center of boxB into boxA's local space
        vec3 relativePos;
        glm_vec3_sub(boxA->globalPos, boxB->globalPos, relativePos);

        // Step 2: Check for overlap on each axis
        float overlapX = halfExtentsA[0] + halfExtentsB[0] - fabs(relativePos[0]);
        float overlapY = halfExtentsA[1] + halfExtentsB[1] - fabs(relativePos[1]);
        float overlapZ = halfExtentsA[2] + halfExtentsB[2] - fabs(relativePos[2]);

        // If there is no overlap on any axis, return no collision
        if (overlapX <= 0 || overlapY <= 0 || overlapZ <= 0) {
            return false; // No collision
        }

        // Step 3: Determine the smallest overlap and set the collision normal
        if (overlapX <= overlapY && overlapX <= overlapZ) {
            collisionNormal[0] = (relativePos[0] < 0) ? -1.0f : 1.0f; // Determine normal direction
            collisionNormal[1] = 0.0f;
            collisionNormal[2] = 0.0f;
            penetrationDepth = overlapX;
        } else if (overlapY <= overlapX && overlapY <= overlapZ) {
            collisionNormal[0] = 0.0f;
            collisionNormal[1] = (relativePos[1] < 0) ? -1.0f : 1.0f; // Determine normal direction
            collisionNormal[2] = 0.0f;
            penetrationDepth = overlapY;
        } else {
            collisionNormal[0] = 0.0f;
            collisionNormal[1] = 0.0f;
            collisionNormal[2] = (relativePos[2] < 0) ? -1.0f : 1.0f; // Determine normal direction
            penetrationDepth = overlapZ;
        }

        glm_vec3_copy(collisionNormal, impactPoint);

        return true; // Collision detected

    } else {

        // Step 1: Extract half-extents and scale them according to global scale
        vec3 halfExtentsA, halfExtentsB;
        glm_vec3_copy(boxA->globalScale, halfExtentsA);
        glm_vec3_copy(boxB->globalScale, halfExtentsB);

        // Step 2: Compute rotation matrices for both boxes

        mat4 rotationMatA = GLM_MAT4_IDENTITY_INIT;
        glm_rotate(rotationMatA, to_radians(boxA->globalRot[0]), (vec3){1.0f, 0.0f, 0.0f});
        glm_rotate(rotationMatA, to_radians(boxA->globalRot[1]), (vec3){0.0f, 1.0f, 0.0f});
        glm_rotate(rotationMatA, to_radians(boxA->globalRot[2]), (vec3){0.0f, 0.0f, 1.0f});

        mat4 rotationMatB = GLM_MAT4_IDENTITY_INIT;
        glm_rotate(rotationMatB, to_radians(boxB->globalRot[0]), (vec3){1.0f, 0.0f, 0.0f});
        glm_rotate(rotationMatB, to_radians(boxB->globalRot[1]), (vec3){0.0f, 1.0f, 0.0f});
        glm_rotate(rotationMatB, to_radians(boxB->globalRot[2]), (vec3){0.0f, 0.0f, 1.0f});

        // Extract 3x3 rotation matrices from the 4x4 matrices
        mat3 rotationA, rotationB;
        glm_mat4_pick3(rotationMatA, rotationA);
        glm_mat4_pick3(rotationMatB, rotationB);

        // Step 3: Compute the translation vector between boxA and boxB
        vec3 translation;
        glm_vec3_sub(boxB->globalPos, boxA->globalPos, translation);

        // Step 4: Define the axes to test (3 from each box, and 9 cross products)
        vec3 axes[15];
        glm_vec3_copy(rotationA[0], axes[0]); // Box A local X-axis
        glm_vec3_copy(rotationA[1], axes[1]); // Box A local Y-axis
        glm_vec3_copy(rotationA[2], axes[2]); // Box A local Z-axis
        glm_vec3_copy(rotationB[0], axes[3]); // Box B local X-axis
        glm_vec3_copy(rotationB[1], axes[4]); // Box B local Y-axis
        glm_vec3_copy(rotationB[2], axes[5]); // Box B local Z-axis

        // Compute cross-product axes (3x3 = 9 axes)
        int axisIndex = 6;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                glm_vec3_cross(rotationA[i], rotationB[j], axes[axisIndex++]);
            }
        }

        // Step 5: SAT Test on each axis
        float minOverlap = FLT_MAX;
        vec3 bestAxis = {0.0f, 0.0f, 0.0f};
        bool collisionDetected = false;

        for (int i = 0; i < 15; ++i) {
            vec3 axis;
            glm_vec3_copy(axes[i], axis);

            // Ignore near-zero axes
            if (glm_vec3_norm(axis) < 1e-6f) continue;

            // Normalize the axis
            glm_vec3_normalize(axis);

            // Project both boxes onto the current axis
            float minA, maxA, minB, maxB;
            project_box_onto_axis(axis, halfExtentsA, rotationA, boxA->globalPos, &minA, &maxA);
            project_box_onto_axis(axis, halfExtentsB, rotationB, boxB->globalPos, &minB, &maxB);

            // Check if projections overlap
            if (!projections_overlap(minA, maxA, minB, maxB)) {
                return false; // No collision on this axis
            }

            // Compute the overlap amount
            float overlap = compute_overlap(minA, maxA, minB, maxB);
            if (overlap < minOverlap) {
                minOverlap = overlap;
                glm_vec3_copy(axis, bestAxis);
                collisionDetected = true;  // Collision detected on this axis
            }
        }

        // Step 6: If we reach here, a collision has occurred
        if (collisionDetected) {
            glm_vec3_copy(bestAxis, collisionNormal);

            penetrationDepth = minOverlap;

            // Compute the impact point
            vec3 scaledNormal;
            glm_vec3_scale(collisionNormal, penetrationDepth, scaledNormal);
            glm_vec3_add(boxA->globalPos, scaledNormal, impactPoint);

            // Adjust the normal direction if necessary
            if (glm_vec3_dot(translation, collisionNormal) > 0) {
                glm_vec3_negate(collisionNormal);  // Flip the normal if it's pointing the wrong way
            }

            apply_collision(boxB, boxA, collisionNormal, impactPoint, penetrationDepth);
            return true;  // Collision detected
        }

        return false;  // No collision detected (should not reach here)
    }
}





bool check_collision_box_with_sphere(Node *shapeA, Node *shapeB) {
    vec3 collisionNormal;
    vec3 impactPoint;
    float penetrationDepth;
    Node *boxShape;
    Node *sphereShape;
    int priorityA, priorityB;
    shapeA::get_priority(&priorityA);
    shapeB::get_priority(&priorityB);
    if (priorityA < priorityB) {
        boxShape = shapeA;
        sphereShape = shapeB;
    } else {
        boxShape = shapeB;
        sphereShape = shapeA;
    }

    // Get sphere properties
    float radius = sphereShape->globalScale[0];

    // Get cube properties
    vec3 cubeHalfExtents;
    glm_vec3_copy(boxShape->globalScale, cubeHalfExtents);
    
    mat4 cubeRotation = GLM_MAT4_IDENTITY_INIT;  // Rotation matrix of the cube
    glm_rotate(cubeRotation, to_radians(boxShape->globalRot[0]), (vec3){1.0f, 0.0f, 0.0f});
    glm_rotate(cubeRotation, to_radians(boxShape->globalRot[1]), (vec3){0.0f, 1.0f, 0.0f});
    glm_rotate(cubeRotation, to_radians(boxShape->globalRot[2]), (vec3){0.0f, 0.0f, 1.0f});

    // Step 1: Calculate the inverse of the cube's rotation matrix
    mat4 inverseCubeRotation;
    glm_mat4_inv(cubeRotation, inverseCubeRotation);

    // Step 2: Transform the sphere's center into the cube's local space
    vec3 localSphereCenter;
    vec3 relativeSphereCenter;
    glm_vec3_sub(sphereShape->globalPos, boxShape->globalPos, relativeSphereCenter);  // sphere center relative to the cube center
    glm_mat4_mulv3(inverseCubeRotation, relativeSphereCenter, 1.0f, localSphereCenter);  // Rotate the relative position

    // Step 3: Perform the AABB vs. sphere collision check
    vec3 closestPoint;
    for (int i = 0; i < 3; i++) {
        // Clamp the sphere's local position to the bounds of the AABB (i.e., the cube's half extents)
        if (localSphereCenter[i] < -cubeHalfExtents[i]) {
            closestPoint[i] = -cubeHalfExtents[i];
        } else if (localSphereCenter[i] > cubeHalfExtents[i]) {
            closestPoint[i] = cubeHalfExtents[i];
        } else {
            closestPoint[i] = localSphereCenter[i];
        }
    }

    // Step 4: Compute the distance between the closest point on the AABB and the sphere's center
    float distanceSquared = glm_vec3_distance2(localSphereCenter, closestPoint);

    // If the distance is less than or equal to the sphere's radius, there is a collision
    if (distanceSquared <= radius * radius) {
        // Calculate the penetrationDepth
        (penetrationDepth) = -(sqrt(distanceSquared) - radius);

        glm_vec3_sub(closestPoint, localSphereCenter, collisionNormal);

        int priorityA, priorityB;
        shapeA::get_priority(&priorityA);
        shapeB::get_priority(&priorityB);
        if (priorityA < priorityB) glm_vec3_negate(collisionNormal);
        glm_vec3_normalize(collisionNormal);

        glm_vec3_rotate_m4(cubeRotation, collisionNormal, collisionNormal);

        // Calculate the impact point
        glm_vec3_scale(collisionNormal, radius, impactPoint);
        glm_vec3_add(sphereShape->globalPos, impactPoint, impactPoint);

        apply_collision(shapeA, shapeB, collisionNormal, impactPoint, penetrationDepth);
        return 1;  // Collision detected
    }

    return 0;  // No collision

}


bool check_collision_box_with_plane(Node *shapeA, Node *shapeB) {
    vec3 collisionNormal;
    vec3 impactPoint;
    float penetrationDepth;
    Node *boxShape;
    Node *planeShape;

    int priorityA, priorityB;
    shapeA::get_priority(&priorityA);
    shapeB::get_priority(&priorityB);
    if (priorityA < priorityB) {
        boxShape = shapeA;
        planeShape = shapeB;
    } else {
        boxShape = shapeB;
        planeShape = shapeA;
    }

    // Get cube properties
    vec3 cubeHalfExtents;
    glm_vec3_copy(boxShape->globalScale, cubeHalfExtents);


    mat4 cubeRotation = GLM_MAT4_IDENTITY_INIT;  // Rotation matrix of the cube
    glm_rotate(cubeRotation, to_radians(boxShape->globalRot[0]), (vec3){1.0f, 0.0f, 0.0f});
    glm_rotate(cubeRotation, to_radians(boxShape->globalRot[1]), (vec3){0.0f, 1.0f, 0.0f});
    glm_rotate(cubeRotation, to_radians(boxShape->globalRot[2]), (vec3){0.0f, 0.0f, 1.0f});

    // Get plane properties
    vec3 planeNormal = {0.0, 1.0, 0.0};
    glm_vec3_rotate(planeNormal, to_radians(planeShape->globalRot[0]), (vec3){1.0f, 0.0f, 0.0f});
    glm_vec3_rotate(planeNormal, to_radians(planeShape->globalRot[1]), (vec3){0.0f, 1.0f, 0.0f});
    glm_vec3_rotate(planeNormal, to_radians(planeShape->globalRot[2]), (vec3){0.0f, 0.0f, 1.0f});
    float planeDistance = glm_vec3_dot(planeNormal, planeShape->globalPos);  // Distance from origin to the plane along the normal

    // Step 1: Define the 8 local vertices of the cube (before rotation/translation)
    vec3 localVertices[8];
    glm_vec3_copy((vec3){-cubeHalfExtents[0], -cubeHalfExtents[1], -cubeHalfExtents[2]}, localVertices[0]);
    glm_vec3_copy((vec3){ cubeHalfExtents[0], -cubeHalfExtents[1], -cubeHalfExtents[2]}, localVertices[1]);
    glm_vec3_copy((vec3){-cubeHalfExtents[0],  cubeHalfExtents[1], -cubeHalfExtents[2]}, localVertices[2]);
    glm_vec3_copy((vec3){ cubeHalfExtents[0],  cubeHalfExtents[1], -cubeHalfExtents[2]}, localVertices[3]);
    glm_vec3_copy((vec3){-cubeHalfExtents[0], -cubeHalfExtents[1],  cubeHalfExtents[2]}, localVertices[4]);
    glm_vec3_copy((vec3){ cubeHalfExtents[0], -cubeHalfExtents[1],  cubeHalfExtents[2]}, localVertices[5]);
    glm_vec3_copy((vec3){-cubeHalfExtents[0],  cubeHalfExtents[1],  cubeHalfExtents[2]}, localVertices[6]);
    glm_vec3_copy((vec3){ cubeHalfExtents[0],  cubeHalfExtents[1],  cubeHalfExtents[2]}, localVertices[7]);

    // Step 2: Transform the cube's local vertices into world space
    vec3 globalVertices[8];
    for (int i = 0; i < 8; i++) {
        vec3 rotatedVertex;
        glm_mat4_mulv3(cubeRotation, localVertices[i], 1.0f, rotatedVertex);  // Apply cube's rotation
        glm_vec3_add(rotatedVertex, boxShape->globalPos, globalVertices[i]);  // Apply cube's translation
    }

    // Step 3: Check the signed distance of each vertex to the plane
    bool positiveSide = false;
    bool negativeSide = false;
    float minDistanceToPlane = 0.0f;
    for (int i = 0; i < 8; i++) {
        float distanceToPlane = glm_vec3_dot(planeNormal, globalVertices[i]) - planeDistance;
        if (distanceToPlane > 0.0f) {
            positiveSide = true;  // Vertex is on the positive side of the plane
        } else if (distanceToPlane < 0.0f) {
            if (distanceToPlane < minDistanceToPlane) minDistanceToPlane = distanceToPlane;
            negativeSide = true;  // Vertex is on the negative side of the plane
        }
    }

    // If we find a vertex on each side, the cube intersects the plane
    if (positiveSide && negativeSide) {

        // Calculate the penetrationDepth
        (penetrationDepth) = -(minDistanceToPlane);

        // Calculate the collision normal (direction of the force)
        glm_vec3_copy(planeNormal, collisionNormal);

        int priorityA, priorityB;
        shapeA::get_priority(&priorityA);
        shapeB::get_priority(&priorityB);
        if (priorityA < priorityB) glm_vec3_negate(collisionNormal);
        glm_vec3_normalize(collisionNormal);
        // Calculate the impact point
        glm_vec3_scale(collisionNormal, penetrationDepth, impactPoint);
        glm_vec3_add(boxShape->globalPos, impactPoint, impactPoint);

        apply_collision(shapeA, shapeB, collisionNormal, impactPoint, penetrationDepth);
        return 1;  // Collision detected
    }

    // If all vertices are on the same side, there is no collision
    return 0;

}

bool check_collision_box_with_capsule(Node *shapeA, Node *shapeB) {
    return 0;
}

bool check_collision_box_with_mesh(Node *shapeA, Node *shapeB) {
    return 0;
}

bool check_collision_box_with_ray(struct Node *shapeA, struct Node *shapeB) {
    Node *boxShape, *rayShape;
    int priorityA, priorityB;
    shapeA::get_priority(&priorityA);
    shapeB::get_priority(&priorityB);
    if (priorityA < priorityB) {
        boxShape = shapeA;
        rayShape = shapeB;
    } else {
        boxShape = shapeB;
        rayShape = shapeA;
    }

    // Get ray properties
    vec3 rayOrigin;
    glm_vec3_copy(rayShape->globalPos, rayOrigin);

    vec3 rayDirection = {0.0f, 0.0f, -1.0f};
    glm_vec3_rotate(rayDirection, to_radians(rayShape->globalRot[0]), (vec3){1.0f, 0.0f, 0.0f});
    glm_vec3_rotate(rayDirection, to_radians(rayShape->globalRot[1]), (vec3){0.0f, 1.0f, 0.0f});
    glm_vec3_rotate(rayDirection, to_radians(rayShape->globalRot[2]), (vec3){0.0f, 0.0f, 1.0f});

    // Get cube properties
    vec3 cubeHalfExtents;
    glm_vec3_copy(boxShape->globalScale, cubeHalfExtents);
    
    mat4 cubeRotation = GLM_MAT4_IDENTITY_INIT;  // Rotation matrix of the cube
    glm_rotate(cubeRotation, to_radians(boxShape->globalRot[0]), (vec3){1.0f, 0.0f, 0.0f});
    glm_rotate(cubeRotation, to_radians(boxShape->globalRot[1]), (vec3){0.0f, 1.0f, 0.0f});
    glm_rotate(cubeRotation, to_radians(boxShape->globalRot[2]), (vec3){0.0f, 0.0f, 1.0f});

    // Step 1: Calculate the inverse of the cube's rotation matrix
    mat4 inverseCubeRotation;
    glm_mat4_inv(cubeRotation, inverseCubeRotation);

    // Step 2: Transform the ray's origin and direction into the cube's local space
    vec3 localRayOrigin, localRayDirection;
    vec3 relativeRayOrigin;
    glm_vec3_sub(rayOrigin, boxShape->globalPos, relativeRayOrigin);  // ray origin relative to the cube center
    glm_mat4_mulv3(inverseCubeRotation, relativeRayOrigin, 1.0f, localRayOrigin);  // Rotate the relative position
    glm_mat4_mulv3(inverseCubeRotation, rayDirection, 0.0f, localRayDirection);  // Rotate the direction

    // Step 3: Perform the AABB vs. ray collision check
    float tMin = -FLT_MAX, tMax = FLT_MAX;
    for (int i = 0; i < 3; i++) {
        if (fabs(localRayDirection[i]) < FLT_EPSILON) {
            // Ray is parallel to the slab. No hit if origin not within slab
            if (localRayOrigin[i] < -cubeHalfExtents[i] || localRayOrigin[i] > cubeHalfExtents[i]) {
                return 0;  // No collision
            }
        } else {
            // Compute intersection t value of ray with near and far plane of slab
            float t1 = (-cubeHalfExtents[i] - localRayOrigin[i]) / localRayDirection[i];
            float t2 = (cubeHalfExtents[i] - localRayOrigin[i]) / localRayDirection[i];
            // Make t1 be intersection with near plane, t2 with far plane
            if (t1 > t2) {
                float temp = t1;
                t1 = t2;
                t2 = temp;
            }
            // Compute the intersection of slab intersection intervals
            if (t1 > tMin) tMin = t1;
            if (t2 < tMax) tMax = t2;
            // Exit with no collision as soon as slab intersection becomes empty
            if (tMin > tMax) {
                
                return 0;  // No collision
            }
        }
    }
    if (tMin < 0) {
        
        return 0;  // No collision
    }

    // If we reach here, there is a collision
    vec3 collisionPoint;
    glm_vec3_scale(localRayDirection, tMin, collisionPoint);
    glm_vec3_add(localRayOrigin, collisionPoint, collisionPoint);

    vec3 collisionNormal = {0.0f, 0.0f, 0.0f};
    for (int i = 0; i < 3; i++) {
        if (fabs(collisionPoint[i] - cubeHalfExtents[i]) < FLT_EPSILON) {
            collisionNormal[i] = 1.0f;
        } else if (fabs(collisionPoint[i] + cubeHalfExtents[i]) < FLT_EPSILON) {
            collisionNormal[i] = -1.0f;
        }
    }

    glm_vec3_rotate_m4(cubeRotation, collisionNormal, collisionNormal);

    apply_collision(shapeA, shapeB, collisionNormal, collisionPoint, tMin);
    return 1;  // Collision detected
}


/**
 * Check if a collision occured between two sphere shapes.
 *
 * @param {Node*} shapeA - The first sphere shape.
 * @param {Node*} shapeB - The second sphere shape.
 * @returns {bool} The collision state.
 */

bool check_collision_sphere_with_sphere(Node *shapeA, Node *shapeB) {
    vec3 collisionNormal;
    vec3 impactPoint;
    float penetrationDepth;

    float radiusA = shapeA->globalScale[0];
    float radiusB = shapeB->globalScale[0];

    float distance = radiusA + radiusB;
    if (glm_vec3_distance2(shapeA->globalPos, shapeB->globalPos) < sqr(distance)) {

        // Calculate the penetrationDepth
        vec3 relative_pos;
        glm_vec3_sub(shapeA->globalPos, shapeB->globalPos, relative_pos);
        float norm = glm_vec3_norm(relative_pos);
        (penetrationDepth) = -(norm - radiusA - radiusB);

        // Calculate the collision normal (direction of the force)
        glm_vec3_sub(shapeB->globalPos, shapeA->globalPos, collisionNormal);
        glm_vec3_normalize(collisionNormal);
        // Calculate the impact point
        glm_vec3_scale(collisionNormal, radiusB, impactPoint);
        glm_vec3_add(shapeB->globalPos, impactPoint, impactPoint);

        apply_collision(shapeA, shapeB, collisionNormal, impactPoint, penetrationDepth);
        return 1;
    }

    return 0;
}

/**
 * Check if a collision occured between a sphere shape and a plane shape.
 *
 * @param {Node*} shapeA - The sphere shape.
 * @param {Node*} shapeB - The plane shape.
 * @returns {bool} The collision state.
 */

bool check_collision_sphere_with_plane(Node *shapeA, Node *shapeB) {
    vec3 collisionNormal;
    vec3 impactPoint;
    float penetrationDepth;
    Node *sphereShape;
    Node *planeShape;
    
    int priorityA, priorityB;
    shapeA::get_priority(&priorityA);
    shapeB::get_priority(&priorityB);
    if (priorityA < priorityB) {
        sphereShape = shapeA;
        planeShape = shapeB;
    } else {
        sphereShape = shapeB;
        planeShape = shapeA;
    }

    float radius = sphereShape->globalScale[0];

    vec3 planeNormal = {0.0, 1.0, 0.0};
    glm_vec3_rotate(planeNormal, to_radians(planeShape->globalRot[0]), (vec3){1.0f, 0.0f, 0.0f});
    glm_vec3_rotate(planeNormal, to_radians(planeShape->globalRot[1]), (vec3){0.0f, 1.0f, 0.0f});
    glm_vec3_rotate(planeNormal, to_radians(planeShape->globalRot[2]), (vec3){0.0f, 0.0f, 1.0f});
    float planeDistance = glm_vec3_dot(planeNormal, planeShape->globalPos);  // Distance from origin to the plane along the normal

    float distanceFromPlane = glm_vec3_dot(planeNormal, sphereShape->globalPos) - planeDistance;
    float absDistanceFromPlane = fabs(distanceFromPlane);

    if (absDistanceFromPlane <= radius) {

        // Calculate the penetrationDepth
        (penetrationDepth) = -(distanceFromPlane - radius);

        // Calculate the collision normal (direction of the force)
        glm_vec3_copy(planeNormal, collisionNormal);

        int priorityA, priorityB;
        shapeA::get_priority(&priorityA);
        shapeB::get_priority(&priorityB);
        if (priorityA < priorityB) glm_vec3_negate(collisionNormal);
        glm_vec3_normalize(collisionNormal);

        // Calculate the impact point
        glm_vec3_scale(collisionNormal, radius - absDistanceFromPlane, impactPoint);
        glm_vec3_add(sphereShape->globalPos, impactPoint, impactPoint);

        apply_collision(shapeA, shapeB, collisionNormal, impactPoint, penetrationDepth);
        return 1;
    }

    return 0;
}


bool check_collision_sphere_with_capsule(Node *shapeA, Node *shapeB) {
    return 0;
}

bool check_collision_sphere_with_mesh(Node *shapeA, Node *shapeB) {
    vec3 collisionNormal;
    vec3 angularNormal;
    float penetrationDepth;
    Node *sphereShape;
    Node *meshShape;
    int priorityA, priorityB;
    shapeA::get_priority(&priorityA);
    shapeB::get_priority(&priorityB);
    if (priorityA < priorityB) {
        sphereShape = shapeA;
        meshShape = shapeB;
    } else {
        sphereShape = shapeB;
        meshShape = shapeA;
    }
    MeshCollisionShape *mesh = (MeshCollisionShape *)meshShape->object;
    float radius = sphereShape->scale[0];

    mat4 meshRotation = GLM_MAT4_IDENTITY_INIT;
    glm_rotate(meshRotation, to_radians(meshShape->globalRot[0]), (vec3){1.0f, 0.0f, 0.0f});
    glm_rotate(meshRotation, to_radians(meshShape->globalRot[1]), (vec3){0.0f, 1.0f, 0.0f});
    glm_rotate(meshRotation, to_radians(meshShape->globalRot[2]), (vec3){0.0f, 0.0f, 1.0f});

    mat4 inverseMeshRotation;
    glm_mat4_inv(meshRotation, inverseMeshRotation);

    bool collisionDetected = false;
    float minPenetrationDepth = FLT_MAX;
    vec3 bestCollisionNormal = {0.0f, 0.0f, 0.0f};

    for (int i = 0; i < mesh->numFaces; ++i) {
        vec3 face[3];
        for (int j = 0; j < 3; ++j) {
            glm_vec3_copy(mesh->facesVertex[j+i*3], face[j]);
            glm_vec3_mul(face[j], meshShape->globalScale, face[j]);
            glm_mat4_mulv3(meshRotation, face[j], 1.0f, face[j]);
            glm_vec3_add(face[j], meshShape->globalPos, face[j]);
        }

        vec3 closestPoint;
        closest_point_on_triangle(sphereShape->globalPos, face[0], face[1], face[2], closestPoint);

        vec3 diff;
        glm_vec3_sub(sphereShape->globalPos, closestPoint, diff);
        float distanceSquared = glm_vec3_norm2(diff);

        if (distanceSquared < radius * radius) {
            float distance = sqrt(distanceSquared);
            float penetration = radius - distance;
            if (penetration < minPenetrationDepth) {
                minPenetrationDepth = penetration;
                glm_vec3_copy(diff, bestCollisionNormal);

                glm_vec3_normalize(bestCollisionNormal);
                glm_vec3_copy(bestCollisionNormal, collisionNormal);
                glm_vec3_copy(bestCollisionNormal, angularNormal);
                penetrationDepth = minPenetrationDepth;
                // Ensure the normal vector is oriented correctly
                vec3 toSphere;
                glm_vec3_sub(sphereShape->globalPos, closestPoint, toSphere);
                if (glm_vec3_dot(collisionNormal, toSphere) < 0.0f) {
                    glm_vec3_negate(collisionNormal);
                }
                if (priorityA < priorityB) glm_vec3_negate(collisionNormal);
                apply_collision(shapeA, shapeB, collisionNormal, angularNormal, penetrationDepth);

                collisionDetected = true;
            }
        }
    }

    if (collisionDetected) {
        return true;
    }
    return false;
}

bool check_collision_sphere_with_ray(struct Node *shapeA, struct Node *shapeB) {
    Node *sphereShape, *rayShape;
    int priorityA, priorityB;
    shapeA::get_priority(&priorityA);
    shapeB::get_priority(&priorityB);
    if (priorityA < priorityB) {
        sphereShape = shapeA;
        rayShape = shapeB;
    } else {
        sphereShape = shapeB;
        rayShape = shapeA;
    }

    // Get ray properties
    vec3 rayOrigin;
    glm_vec3_copy(rayShape->globalPos, rayOrigin);

    vec3 rayDirection = {0.0f, 0.0f, -1.0f};
    glm_vec3_rotate(rayDirection, to_radians(rayShape->globalRot[0]), (vec3){1.0f, 0.0f, 0.0f});
    glm_vec3_rotate(rayDirection, to_radians(rayShape->globalRot[1]), (vec3){0.0f, 1.0f, 0.0f});
    glm_vec3_rotate(rayDirection, to_radians(rayShape->globalRot[2]), (vec3){0.0f, 0.0f, 1.0f});

    // Get sphere properties
    vec3 sphereCenter;
    glm_vec3_copy(sphereShape->globalPos, sphereCenter);
    float sphereRadius = sphereShape->globalScale[0];  // Assuming uniform scaling

    // Compute the vector from the ray origin to the sphere center
    vec3 oc;
    glm_vec3_sub(rayOrigin, sphereCenter, oc);

    // Compute the coefficients of the quadratic equation
    float a = glm_vec3_dot(rayDirection, rayDirection);
    float b = 2.0f * glm_vec3_dot(oc, rayDirection);
    float c = glm_vec3_dot(oc, oc) - sphereRadius * sphereRadius;

    // Compute the discriminant
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        
        return 0;  // No collision
    } else {
        float t = (-b - sqrt(discriminant)) / (2.0f * a);
        if (t < 0) {
            t = (-b + sqrt(discriminant)) / (2.0f * a);
        }
        if (t < 0) {
            
            return 0;  // No collision
        }

        // Compute the collision point
        vec3 collisionPoint;
        glm_vec3_scale(rayDirection, t, collisionPoint);
        glm_vec3_add(rayOrigin, collisionPoint, collisionPoint);

        // Compute the collision normal
        vec3 collisionNormal;
        glm_vec3_sub(collisionPoint, sphereCenter, collisionNormal);
        glm_vec3_normalize(collisionNormal);
        
        apply_collision(shapeA, shapeB, collisionNormal, collisionPoint, t);
        return 1;  // Collision detected
    }
}


bool check_collision_plane_with_plane(Node *shapeA, Node *shapeB) {
    return 0;
}

bool check_collision_plane_with_capsule(Node *shapeA, Node *shapeB) {
    return 0;
}

bool check_collision_plane_with_mesh(Node *shapeA, Node *shapeB) {
    return 0;
}

bool check_collision_plane_with_ray(Node *shapeA, Node *shapeB) {
    return 0;
}

bool check_collision_capsule_with_capsule(Node *shapeA, Node *shapeB) {
    return 0;
}

bool check_collision_capsule_with_mesh(Node *shapeA, Node *shapeB) {
    return 0;
}

bool check_collision_capsule_with_ray(Node *shapeA, Node *shapeB) {
    return 0;
}

bool check_collision_mesh_with_mesh(Node *shapeA, Node *shapeB) {
    return 0;
}

bool check_collision_mesh_with_ray(Node *shapeA, Node *shapeB) {
    return 0;
}

bool check_collision_ray_with_ray(Node *shapeA, Node *shapeB) {
    return 0;
}