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
#include "../classes/classes.h"
#include "../memory.h"
#include "../buffer.h"
#include "physics.h"
#include "bodies.h"
#include "collision.h"
#include "collision_util.h"


bool check_collision_sphere_with_mesh(Node *shapeA, Node *shapeB) {
    vec3 collisionNormal;
    vec3 angularNormal;
    float penetrationDepth;
    Node *sphereShape;
    Node *meshShape;
    int priorityA, priorityB;
    METHOD(shapeA, get_priority, &priorityA);
    METHOD(shapeB, get_priority, &priorityB);
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
            glm_vec3_copy(mesh->facesVertex[i][j], face[j]);
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

                printf("Collision detected\n");
                collisionDetected = true;
            }
        }
    }

    if (collisionDetected) {
        return true;
    }
    printf("No collision detected\n");
    return false;
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