#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "../types.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../node.h"
#include "../window.h"
#include "../render/color.h"
#include "../render/camera.h"
#include "../render/depth_map.h"
#include "../render/viewport.h"
#include "../render/lighting.h"
#include "../io/gltexture_loader.h"
#include "../memory.h"
#include "physics.h"
#include "bodies.h"
#include "collision.h"
#include "cube.h"

/**
 * Apply force vector to a node.
 *
 * @param {Node*} node - The affected node.
 * @param {vec3} origin - Node absolute position.
 * @param {vec3} force - Force vector.
 * @param {float} distance - Distance from force origin.
 */

void apply_impulse(Node *node, vec3 impulse, vec3 torque, vec3 correction, float momentOfInertia) {
    switch (node->type) {
        case NODE_RIGID_BODY:
            RigidBody *rigidBody = (RigidBody *) node->object;

            // Change velocity
            glm_vec3_add(rigidBody->velocity, impulse, rigidBody->velocity);

            // Move shape A out of shape B
            glm_vec3_sub(node->pos, correction, node->pos);
            for (int i = 0; i < rigidBody->length; i++) {
                glm_vec3_sub(rigidBody->collisionsShapes[i]->globalPos, correction, rigidBody->collisionsShapes[i]->globalPos);
            }

            // Apply torque
            glm_vec3_scale(torque, momentOfInertia, torque);
            glm_vec3_sub(rigidBody->angularVelocity, torque, rigidBody->angularVelocity);

        break;
        case NODE_KINEMATIC_BODY:
            KinematicBody *kinematicBody = (KinematicBody *) node->object;

            // Change velocity
            glm_vec3_add(kinematicBody->velocity, impulse, kinematicBody->velocity);

            // Move shape A out of shape B
            glm_vec3_sub(node->pos, correction, node->pos);
            for (int i = 0; i < kinematicBody->length; i++) {
                glm_vec3_sub(kinematicBody->collisionsShapes[i]->globalPos, correction, kinematicBody->collisionsShapes[i]->globalPos);
            }
        break;
    }
};

/**
 * Get the velocity's norm of a node.
 *
 * @param {Node*} node - The affected node.
 * @returns {float} The velocity's norm.
 */

float get_velocity_norm(Node *node) {
    switch (node->type) {
        case NODE_KINEMATIC_BODY:
            KinematicBody *kinematicBody = (KinematicBody *) node->object;
            return glm_vec3_norm(kinematicBody->velocity);
        case NODE_RIGID_BODY:
            RigidBody *rigidBody = (RigidBody *) node->object;
            return glm_vec3_norm(rigidBody->velocity);
    }
    return 0.0f;
};

/**
 * Get the velocity of a node.
 *
 * @param {Node*} node - The affected node.
 * @returns {float} The velocity.
 */

void get_velocity(Node *node, vec3 velocity) {
    switch (node->type) {
        case NODE_KINEMATIC_BODY:
            KinematicBody *kinematicBody = (KinematicBody *) node->object;
            glm_vec3_copy(kinematicBody->velocity, velocity);
        break;
        case NODE_RIGID_BODY:
            RigidBody *rigidBody = (RigidBody *) node->object;
            glm_vec3_copy(rigidBody->velocity, velocity);
        break;
        case NODE_STATIC_BODY:
            glm_vec3_zero(velocity);
        break;
    }
};


void get_mass(Node *node, float * mass) {
    switch (node->type) {
        case NODE_RIGID_BODY:
            RigidBody *rigidBody = (RigidBody *) node->object;
            (*mass) = rigidBody->mass;
        break;
        case NODE_STATIC_BODY:
            (*mass) = INFINITY;
        break;
        case NODE_KINEMATIC_BODY:
            (*mass) = 100.0;
        break;
    }
}

void get_center_of_mass(Node *node, vec3 com) {
    switch (node->type) {
        case NODE_RIGID_BODY:
            RigidBody *rigidBody = (RigidBody *) node->object;
            glm_vec3_copy(rigidBody->centerOfMass, com);
        break;
        case NODE_STATIC_BODY:
            glm_vec3_zero(com);
        break;
        case NODE_KINEMATIC_BODY:
            glm_vec3_zero(com);
        break;
    }
}



// Main SAT algorithm for box-box collision detection
bool check_collision_box_with_box(Node *boxB, Node *boxA, vec3 collisionNormal, vec3 angularNormal, float *penetrationDepth) {

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
            *penetrationDepth = overlapX;
        } else if (overlapY <= overlapX && overlapY <= overlapZ) {
            collisionNormal[0] = 0.0f;
            collisionNormal[1] = (relativePos[1] < 0) ? -1.0f : 1.0f; // Determine normal direction
            collisionNormal[2] = 0.0f;
            *penetrationDepth = overlapY;
        } else {
            collisionNormal[0] = 0.0f;
            collisionNormal[1] = 0.0f;
            collisionNormal[2] = (relativePos[2] < 0) ? -1.0f : 1.0f; // Determine normal direction
            *penetrationDepth = overlapZ;
        }

        glm_vec3_copy(collisionNormal, angularNormal);

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
            glm_vec3_copy(bestAxis, angularNormal);
            *penetrationDepth = minOverlap;

            // Adjust the normal direction if necessary
            if (glm_vec3_dot(translation, collisionNormal) > 0) {
                glm_vec3_negate(collisionNormal);  // Flip the normal if it's pointing the wrong way
            }

            return true;  // Collision detected
        }

        return false;  // No collision detected (should not reach here)
    }
}





bool check_collision_box_with_sphere(Node *shapeA, Node *shapeB, vec3 collisionNormal, vec3 angularNormal, float *penetrationDepth) {
    Node *boxShape;
    Node *sphereShape;
    if (shapeA->type < shapeB->type) {
        boxShape = shapeA;
        sphereShape = shapeB;
    } else {
        boxShape = shapeB;
        sphereShape = shapeA;
    }

    // Get sphere properties
    float radius = sphereShape->scale[0];

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
        (*penetrationDepth) = -(sqrt(distanceSquared) - radius);

        glm_vec3_sub(closestPoint, localSphereCenter, collisionNormal);
        if (shapeA->type < shapeB->type) glm_vec3_negate(collisionNormal);
        glm_vec3_normalize(collisionNormal);

        glm_vec3_rotate_m4(cubeRotation, collisionNormal, collisionNormal);

        glm_vec3_copy(collisionNormal, angularNormal);
        return 1;  // Collision detected
    }

    return 0;  // No collision

}


bool check_collision_box_with_plane(Node *shapeA, Node *shapeB, vec3 collisionNormal, vec3 angularNormal, float *penetrationDepth) {
    Node *boxShape;
    Node *planeShape;
    if (shapeA->type < shapeB->type) {
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
        (*penetrationDepth) = -(minDistanceToPlane);

        // Calculate the collision normal (direction of the force)
        glm_vec3_copy(planeNormal, collisionNormal);
        if (shapeA->type < shapeB->type) glm_vec3_negate(collisionNormal);
        glm_vec3_normalize(collisionNormal);
        glm_vec3_copy(collisionNormal, angularNormal);

        return 1;  // Collision detected
    }

    // If all vertices are on the same side, there is no collision
    return 0;

}


/**
 * Check if a collision occured between two sphere shapes.
 *
 * @param {Node*} shapeA - The first sphere shape.
 * @param {Node*} shapeB - The second sphere shape.
 * @returns {bool} The collision state.
 */

bool check_collision_sphere_with_sphere(Node *shapeA, Node *shapeB, vec3 collisionNormal, vec3 angularNormal, float *penetrationDepth) {

    float radiusA = shapeA->scale[0];
    float radiusB = shapeB->scale[0];

    float distance = radiusA + radiusB;
    if (glm_vec3_distance2(shapeA->globalPos, shapeB->globalPos) < sqr(distance)) {

        // Calculate the penetrationDepth
        vec3 relative_pos;
        glm_vec3_sub(shapeA->globalPos, shapeB->globalPos, relative_pos);
        float norm = glm_vec3_norm(relative_pos);
        (*penetrationDepth) = -(norm - radiusA - radiusB);

        // Calculate the collision normal (direction of the force)
        glm_vec3_sub(shapeB->globalPos, shapeA->globalPos, collisionNormal);
        glm_vec3_normalize(collisionNormal);
        glm_vec3_copy(collisionNormal, angularNormal);

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

bool check_collision_sphere_with_plane(Node *shapeA, Node *shapeB, vec3 collisionNormal, vec3 angularNormal, float *penetrationDepth) {
    Node *sphereShape;
    Node *planeShape;
    if (shapeA->type < shapeB->type) {
        sphereShape = shapeA;
        planeShape = shapeB;
    } else {
        sphereShape = shapeB;
        planeShape = shapeA;
    }

    float radius = sphereShape->scale[0];

    vec3 planeNormal = {0.0, 1.0, 0.0};
    float planeDistance = glm_vec3_dot(planeNormal, planeShape->globalPos);  // Distance from origin to the plane along the normal

    float distanceFromPlane = glm_vec3_dot(planeNormal, sphereShape->globalPos) - planeDistance;
    float absDistanceFromPlane = fabs(distanceFromPlane);

    if (absDistanceFromPlane <= radius) {

        // Calculate the penetrationDepth
        (*penetrationDepth) = -(distanceFromPlane - radius);

        // Calculate the collision normal (direction of the force)
        glm_vec3_copy(planeNormal, collisionNormal);
        if (shapeA->type < shapeB->type) glm_vec3_negate(collisionNormal);
        glm_vec3_normalize(collisionNormal);
        glm_vec3_copy(collisionNormal, angularNormal);

        return 1;
    }

    return 0;
}


bool check_collision_plane_with_plane(Node *shapeA, Node *shapeB, vec3 collisionNormal, vec3 angularNormal, float *penetrationDepth) {
    return 0;
}

/**
 * Return a collision code to determine which function will resolve the collision check.
 *
 * @param {Node*} shapeA - The first shape.
 * @param {Node*} shapeB - The second shape.
 * @returns {unsigned int} The collision code.
 */

unsigned int get_collision_code(Node *shapeA, Node *shapeB) {
    if (shapeA->type < shapeB->type)
        return shapeA->type | shapeB->type << 8;
    return shapeB->type | shapeA->type << 8;
}

/**
 * Check and apply the possibles collisions of the current shape with the collision buffer.
 *
 * @param {Node*} shape - The affected shape.
 * @param {CollisionBuffer*} collisionBuffer - The collision buffer.
 */

void check_collisions(Node *shape, CollisionBuffer *collisionBuffer) {
    
    for (int i = 0; i < collisionBuffer->length; i++) {
        Node *shapeA = shape;
        Node *shapeB = collisionBuffer->collisionsShapes[i];

        vec3 collisionNormal;
        vec3 angularNormal;
        float penetrationDepth;

        bool (*condition)(Node *shapeA, Node *shapeB, vec3 collisionNormal, vec3 angularNormal, float *penetrationDepth);

        switch (get_collision_code(shapeA, shapeB)) {
            case CTEST_BOX_WITH_BOX:
                condition = check_collision_box_with_box;
            break;
            case CTEST_BOX_WITH_SPHERE:
                condition = check_collision_box_with_sphere;
            break;
            case CTEST_BOX_WITH_PLANE:
                condition = check_collision_box_with_plane;
            break;
            case CTEST_SPHERE_WITH_SPHERE:
                condition = check_collision_sphere_with_sphere;
            break;
            case CTEST_SPHERE_WITH_PLANE:
                condition = check_collision_sphere_with_plane;
            break;
            case CTEST_PLANE_WITH_PLANE:
                condition = check_collision_plane_with_plane;
            break;
        }

        if (condition(shapeA, shapeB, collisionNormal, angularNormal, &penetrationDepth)) {

            /*
             * This part has been created with the help of GPT 3.0
             */

            // Mass calculation
            float massA, massB;
            get_mass(shapeA->parent, &massA);
            get_mass(shapeB->parent, &massB);

            // Calculate relative velocity
            vec3 velocityA, velocityB;
            get_velocity(shapeA->parent, velocityA);
            get_velocity(shapeB->parent, velocityB);

            // Calculate the relative velocity between the two bodies
            vec3 relativeVelocity;
            glm_vec3_sub(velocityB, velocityA, relativeVelocity);

            // Calculate relative velocity along the normal direction
            float velocityAlongNormal = glm_vec3_dot(relativeVelocity, collisionNormal);

            // Check if objects are separating (no collision response needed)
            if (velocityAlongNormal > 0) {
                continue; // No collision if they're moving apart
            }

            // Calculate the coefficient of restitution (elasticity of the collision)
            float restitution = 0.02; // Adjust based on how "bouncy" the materials are

            // Calculate the impulse scalar (magnitude)
            float invMassA = (massA == 0) ? 0 : 1.0 / massA;
            float invMassB = (massB == 0) ? 0 : 1.0 / massB;
            float impulseScalar = (1 + restitution) * velocityAlongNormal;
            impulseScalar /= invMassA + invMassB;

            // Calculate the impulse vector
            vec3 impulse;
            glm_vec3_scale(collisionNormal, impulseScalar, impulse);

            // Get the center of mass
            vec3 comA, comB;
            get_center_of_mass(shapeA->parent, comA);
            get_center_of_mass(shapeB->parent, comB);

            // Calculate the point where the impulse is applied relative to the center of mass
            vec3 rA, rB;
            glm_vec3_sub(angularNormal, comA, rA);  // Vector from COM to point of impact for A
            glm_vec3_sub(comB, angularNormal, rB);  // Same for B

            // Torque generated by the impulse (cross product of r and impulse)
            vec3 torqueA, torqueB;
            glm_vec3_cross(rA, impulse, torqueA);  // Torque on A
            glm_vec3_cross(rB, impulse, torqueB);  // Torque on B (if not static)

            // Apply the impulse to both bodies
            vec3 impulseA, impulseB;
            glm_vec3_scale(impulse, invMassA, impulseA); // Affected by massA
            glm_vec3_scale(impulse, invMassB, impulseB); // Affected by massB


            vec3 correction;
            if (penetrationDepth > 0.0f) {
                glm_vec3_scale(collisionNormal, penetrationDepth, correction);
            }
            else
                glm_vec3_zero(correction);

            apply_impulse(shapeA->parent, impulseA, torqueA, correction, sqr(invMassA)*500.0f);
            glm_vec3_negate(impulseB);
            glm_vec3_negate(correction);
            apply_impulse(shapeB->parent, impulseB, torqueB, correction, sqr(invMassB)*500.0f);

        }
    }
}

/**
 * Set the relative position to the global position computed by physics inheritance tree 
 * (mandatory for bodies to work properly).
 *
 * @param {Node*} node - The affected node.
 * @param {vec3} pos - The computed position by physics inheritance tree.
 * @param {vec3} rot - The computed rotation by physics inheritance tree.
 * @param {vec3} scale - The computed scale by physics inheritance tree.
 */

void update_global_position(Node *node, vec3 pos, vec3 rot, vec3 scale) {
    vec3 nodePos;
    glm_vec3_copy(node->pos, nodePos);

    glm_vec3_mul(nodePos, scale, nodePos);

    glm_vec3_rotate(nodePos, to_radians(rot[0]), (vec3){1.0f, 0.0f, 0.0f});
    glm_vec3_rotate(nodePos, to_radians(rot[1]), (vec3){0.0f, 1.0f, 0.0f});
    glm_vec3_rotate(nodePos, to_radians(rot[2]), (vec3){0.0f, 0.0f, 1.0f});

    glm_vec3_add(pos, nodePos, pos);

    glm_vec3_add(rot, node->rot, rot);
    glm_vec3_mul(scale, node->scale, scale);
    glm_vec3_copy(pos, node->globalPos);
    glm_vec3_copy(rot, node->globalRot);
    glm_vec3_copy(scale, node->globalScale);
}

/**
 * Update a static body in physics world
 *
 * @param {Node*} node - The affected node.
 * @param {vec3} pos - The computed position by physics inheritance tree.
 * @param {vec3} rot - The computed rotation by physics inheritance tree.
 * @param {vec3} scale - The computed scale by physics inheritance tree.
 * @param {vec3} delta - Elapsed time since the game last frame.
 */

void update_static_body(Node *node, vec3 pos, vec3 rot, vec3 scale, float delta, CollisionBuffer *collisionBuffer) {
    StaticBody *staticBody = (StaticBody *) node->object;

    update_global_position(node, pos, rot, scale);

    for (int i = 0; i < staticBody->length; i++) {
        update_global_position(staticBody->collisionsShapes[i], pos, rot, scale);
        glm_vec3_copy(node->globalPos, pos);
        glm_vec3_copy(node->globalRot, rot);
        glm_vec3_copy(node->globalScale, scale);
        check_collisions(staticBody->collisionsShapes[i], collisionBuffer);
    }
    for (int i = 0; i < staticBody->length; i++) {
        collisionBuffer->collisionsShapes[collisionBuffer->length++] = staticBody->collisionsShapes[i];
    }
}

/**
 * Update a rigid body in physics world
 *
 * @param {Node*} node - The affected node.
 * @param {vec3} pos - The computed position by physics inheritance tree.
 * @param {vec3} rot - The computed rotation by physics inheritance tree.
 * @param {vec3} scale - The computed scale by physics inheritance tree.
 * @param {vec3} delta - Elapsed time since the game last frame.
 */

void update_rigid_body(Node *node, vec3 pos, vec3 rot, vec3 scale, float delta, CollisionBuffer *collisionBuffer) {
    RigidBody *rigidBody = (RigidBody *) node->object;

    vec3 gravity;
    glm_vec3_scale(rigidBody->gravity, delta, gravity);
    glm_vec3_add(rigidBody->velocity, gravity, rigidBody->velocity);
    glm_vec3_scale(rigidBody->velocity, rigidBody->friction * (1.0-delta), rigidBody->velocity);
    glm_vec3_add(node->pos, rigidBody->velocity, node->pos);

    glm_vec3_scale(rigidBody->angularVelocity, rigidBody->friction * (1.0-delta), rigidBody->angularVelocity);
    glm_vec3_add(node->rot, rigidBody->angularVelocity, node->rot);


    update_global_position(node, pos, rot, scale);


    for (int i = 0; i < rigidBody->length; i++) {
        update_global_position(rigidBody->collisionsShapes[i], pos, rot, scale);
        glm_vec3_copy(node->globalPos, pos);
        glm_vec3_copy(node->globalRot, rot);
        glm_vec3_copy(node->globalScale, scale);
        check_collisions(rigidBody->collisionsShapes[i], collisionBuffer);
    }
    for (int i = 0; i < rigidBody->length; i++) {
        collisionBuffer->collisionsShapes[collisionBuffer->length++] = rigidBody->collisionsShapes[i];
    }
}

/**
 * Update a rigid body in physics world
 *
 * @param {Node*} node - The affected node.
 * @param {vec3} pos - The computed position by physics inheritance tree.
 * @param {vec3} rot - The computed rotation by physics inheritance tree.
 * @param {vec3} scale - The computed scale by physics inheritance tree.
 * @param {vec3} delta - Elapsed time since the game last frame.
 */

void update_kinematic_body(Node *node, vec3 pos, vec3 rot, vec3 scale, float delta, CollisionBuffer *collisionBuffer) {
    KinematicBody *kinematicBody = (KinematicBody *) node->object;

    glm_vec3_add(node->pos, kinematicBody->velocity, node->pos);
    update_global_position(node, pos, rot, scale);


    for (int i = 0; i < kinematicBody->length; i++) {
        update_global_position(kinematicBody->collisionsShapes[i], pos, rot, scale);
        glm_vec3_copy(node->globalPos, pos);
        glm_vec3_copy(node->globalRot, rot);
        glm_vec3_copy(node->globalScale, scale);
        check_collisions(kinematicBody->collisionsShapes[i], collisionBuffer);
    }
    for (int i = 0; i < kinematicBody->length; i++) {
        collisionBuffer->collisionsShapes[collisionBuffer->length++] = kinematicBody->collisionsShapes[i];
    }
}

/**
 * Set graphics camera to physics camera global position
 *
 * @param {Node*} node - The physics camera node.
 * @param {vec3} pos - The computed position by physics inheritance tree.
 * @param {vec3} rot - The computed rotation by physics inheritance tree.
 * @param {vec3} scale - The computed scale by physics inheritance tree.
 * @param {vec3} delta - Elapsed time since the game last frame.
 */

void update_camera(Node *node, vec3 pos, vec3 rot, vec3 scale, float delta, CollisionBuffer *collisionBuffer /*unused*/) {
    Camera *camera = (Camera *) node->object;

    update_global_position(node, pos, rot, scale);

    glm_vec3_copy(pos, camera->pos);
    glm_vec3_copy(rot, camera->rot);
    camera->dir[0] = sin(-camera->rot[1] * PI/180);
	camera->dir[1] = camera->rot[0] * PI/180;
    camera->dir[2] = -cos(-camera->rot[1] * PI/180);
}

/**
 * Update a point light
 *
 * @param {Node*} node - The point light node.
 * @param {vec3} pos - The computed position by physics inheritance tree.
 * @param {vec3} rot - The computed rotation by physics inheritance tree.
 * @param {vec3} scale - The computed scale by physics inheritance tree.
 * @param {vec3} delta - Elapsed time since the game last frame.
 */

void update_point_light(Node *node, vec3 pos, vec3 rot, vec3 scale, float delta, CollisionBuffer *collisionBuffer /*unused*/, u8 lightsCount[LIGHTS_COUNT]) {
    PointLight *pointLight = (PointLight *) node->object;

    update_global_position(node, pos, rot, scale);

    const char uniforms[7][20] = {
        "].position",
        "].ambient",
        "].diffuse",
        "].specular",
        "].constant",
        "].linear",
        "].quadratic"
    };

    char uniformsName[7][50];

    for (int i = 0; i < 7; i++) {
        strcpy(uniformsName[i], "pointLights[");
        sprintf(uniformsName[i] + strlen(uniformsName[i]), "%d", lightsCount[POINT_LIGHT]);
        strcat(uniformsName[i], uniforms[i]);
    }

    for (int i = 0; i < memoryCaches.shadersCount; i++) {
        use_shader(memoryCaches.shaderCache[i].shader);
        set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[0], node->globalPos);
        set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[1], pointLight->ambient);
        set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[2], pointLight->diffuse);
        set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[3], pointLight->specular);
        set_shader_float(memoryCaches.shaderCache[i].shader, uniformsName[4], pointLight->constant);
        set_shader_float(memoryCaches.shaderCache[i].shader, uniformsName[5], pointLight->linear);
        set_shader_float(memoryCaches.shaderCache[i].shader, uniformsName[6], pointLight->quadratic);
    }
    lightsCount[POINT_LIGHT]++;

}

/**
 * Update a directional light
 *
 * @param {Node*} node - The directional light node.
 * @param {vec3} pos - The computed position by physics inheritance tree.
 * @param {vec3} rot - The computed rotation by physics inheritance tree.
 * @param {vec3} scale - The computed scale by physics inheritance tree.
 * @param {vec3} delta - Elapsed time since the game last frame.
 */

void update_directional_light(Node *node, vec3 pos, vec3 rot, vec3 scale, float delta, CollisionBuffer *collisionBuffer /*unused*/, u8 lightsCount[LIGHTS_COUNT]) {
    DirectionalLight *directionalLight = (DirectionalLight *) node->object;

    update_global_position(node, pos, rot, scale);

    lightsCount[DIRECTIONAL_LIGHT]++;

    for (int i = 0; i < memoryCaches.shadersCount; i++) {
        use_shader(memoryCaches.shaderCache[i].shader);
        set_shader_vec3(memoryCaches.shaderCache[i].shader, "pointLights[0].pos", pos);
        set_shader_vec3(memoryCaches.shaderCache[i].shader, "pointLights[0].ambient", directionalLight->ambient);
        set_shader_vec3(memoryCaches.shaderCache[i].shader, "pointLights[0].diffuse", directionalLight->diffuse);
        set_shader_vec3(memoryCaches.shaderCache[i].shader, "pointLights[0].specular", directionalLight->specular);
        set_shader_float(memoryCaches.shaderCache[i].shader, "pointLights[0].constant", directionalLight->constant);
        set_shader_float(memoryCaches.shaderCache[i].shader, "pointLights[0].linear", directionalLight->linear);
        set_shader_float(memoryCaches.shaderCache[i].shader, "pointLights[0].quadratic", directionalLight->quadratic);
    }

}

/**
 * Update a spot light
 *
 * @param {Node*} node - The spot light node.
 * @param {vec3} pos - The computed position by physics inheritance tree.
 * @param {vec3} rot - The computed rotation by physics inheritance tree.
 * @param {vec3} scale - The computed scale by physics inheritance tree.
 * @param {vec3} delta - Elapsed time since the game last frame.
 */

void update_spot_light(Node *node, vec3 pos, vec3 rot, vec3 scale, float delta, CollisionBuffer *collisionBuffer /*unused*/, u8 lightsCount[LIGHTS_COUNT]) {
    SpotLight *spotLight = (SpotLight *) node->object;

    update_global_position(node, pos, rot, scale);

    const char uniforms[10][20] = {
        "].position",
        "].direction",
        "].ambient",
        "].diffuse",
        "].specular",
        "].constant",
        "].linear",
        "].quadratic",
        "].cutOff",
        "].outerCutOff"
    };

    char uniformsName[10][50];

    for (int i = 0; i < 10; i++) {
        strcpy(uniformsName[i], "spotLights[");
        sprintf(uniformsName[i] + strlen(uniformsName[i]), "%d", lightsCount[SPOT_LIGHT]);
        strcat(uniformsName[i], uniforms[i]);
    }

    vec3 dir = {1.0, 0.0, 0.0};

    glm_vec3_rotate(dir, to_radians(rot[0]), (vec3){1.0f, 0.0f, 0.0f});
    glm_vec3_rotate(dir, to_radians(rot[1]), (vec3){0.0f, 1.0f, 0.0f});
    glm_vec3_rotate(dir, to_radians(rot[2]), (vec3){0.0f, 0.0f, 1.0f});

    for (int i = 0; i < memoryCaches.shadersCount; i++) {
        use_shader(memoryCaches.shaderCache[i].shader);
        set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[0], node->globalPos);
        set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[1], dir);
        set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[2], spotLight->ambient);
        set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[3], spotLight->diffuse);
        set_shader_vec3(memoryCaches.shaderCache[i].shader, uniformsName[4], spotLight->specular);
        set_shader_float(memoryCaches.shaderCache[i].shader, uniformsName[5], spotLight->constant);
        set_shader_float(memoryCaches.shaderCache[i].shader, uniformsName[6], spotLight->linear);
        set_shader_float(memoryCaches.shaderCache[i].shader, uniformsName[7], spotLight->quadratic);
        set_shader_float(memoryCaches.shaderCache[i].shader, uniformsName[8], spotLight->cutOff);
        set_shader_float(memoryCaches.shaderCache[i].shader, uniformsName[9], spotLight->outerCutOff);
    }
    lightsCount[SPOT_LIGHT]++;

}

/**
 * Update node in the physics world
 *
 * @param {Node*} node - The affected node.
 * @param {vec3} pos - The computed position by physics inheritance tree.
 * @param {vec3} rot - The computed rotation by physics inheritance tree.
 * @param {vec3} scale - The computed scale by physics inheritance tree.
 * @param {vec3} delta - Elapsed time since the game last frame.
 * @param {CollisionBuffer*} collisionBuffer - The collision buffer.
 */

void update_node_physics(Node *node, vec3 pos, vec3 rot, vec3 scale, float delta, CollisionBuffer *collisionBuffer, u8 lightsCount[LIGHTS_COUNT]) {
    switch (node->type) {
        case NODE_RIGID_BODY:
            update_rigid_body(node, pos, rot, scale, delta, collisionBuffer);
        break;
        case NODE_STATIC_BODY:
            update_static_body(node, pos, rot, scale, delta, collisionBuffer);
        break;
        case NODE_KINEMATIC_BODY:
            update_kinematic_body(node, pos, rot, scale, delta, collisionBuffer);
        break;
        case NODE_CAMERA:
            update_camera(node, pos, rot, scale, delta, collisionBuffer);
        break;
        case NODE_POINT_LIGHT:
            update_point_light(node, pos, rot, scale, delta, collisionBuffer, lightsCount);
        break;
        case NODE_DIRECTIONAL_LIGHT:
            update_directional_light(node, pos, rot, scale, delta, collisionBuffer, lightsCount);
        break;
        case NODE_SPOT_LIGHT:
            update_spot_light(node, pos, rot, scale, delta, collisionBuffer, lightsCount);
        break;
        default:
            update_global_position(node, pos, rot, scale);
        break;
    }
}

void update_script(Node *node, vec3 pos, vec3 rot, vec3 scale, float delta, CollisionBuffer *collisionBuffer, Input *input, Window *window) {
    if (node->flags & NODE_SCRIPT) node->script(node, input, window, delta);
}

/**
 * Update the physics world
 *
 * @param {Node*} node - The selected node by physics inheritance tree.
 * @param {vec3} pos - The computed position by physics inheritance tree.
 * @param {vec3} rot - The computed rotation by physics inheritance tree.
 * @param {vec3} scale - The computed scale by physics inheritance tree.
 * @param {vec3} delta - Elapsed time since the game last frame.
 * @param {CollisionBuffer*} collisionBuffer - The collision buffer.
 */

void update_physics(Node *node, vec3 pos, vec3 rot, vec3 scale, float delta, CollisionBuffer *collisionBuffer, Input *input, Window *window, u8 lightsCount[LIGHTS_COUNT], bool active) {
    vec3 newPos;
    vec3 newRot;
    vec3 newScale;

    glm_vec3_copy(pos, newPos);
    glm_vec3_copy(rot, newRot);
    glm_vec3_copy(scale, newScale);

    if (node->flags & NODE_ACTIVE && (active || node->flags & NODE_EDITOR_FLAG)) {
        update_script(node, newPos, newRot, newScale, delta, collisionBuffer, input, window);
        update_node_physics(node, newPos, newRot, newScale, delta, collisionBuffer, lightsCount);
    } else {
        active = false;
        update_global_position(node, newPos, newRot, newScale);
    }

    for (int i = 0; i < node->length; i++)
        update_physics(node->children[i], newPos, newRot, newScale, delta, collisionBuffer, input, window, lightsCount, active);
}