#pragma once

/**
 * @defgroup Collision Collision Detection and Response
 * @brief Functions and enumerations for collision detection and response in the physics engine.
 *
 * This module provides functions and enumerations to handle collision detection
 * and response between various shapes in the physics engine. It includes functions
 * to check for collisions between different shape types and apply appropriate
 * collision responses.
 * @{
 */


/**
 * @enum CollisionsCode
 * @brief Enumeration for collision codes between different shape types.
 *
 * This enumeration defines various collision codes used to identify
 * the type of collision between different shapes in the physics engine.
 * Each collision code is a combination of two shape types.
 *
 * The shape types are combined using bitwise OR and left shift operations.
 * The lower 8 bits represent the first shape type, and the upper 8 bits
 * represent the second shape type.
 */
enum CollisionsCode {
    CTEST_BOX_WITH_BOX              = CLASS_TYPE_BOXCSHAPE | CLASS_TYPE_BOXCSHAPE << 8, /**< Collision between two box shapes. */
    CTEST_BOX_WITH_SPHERE           = CLASS_TYPE_BOXCSHAPE | CLASS_TYPE_SPHERECSHAPE << 8, /**< Collision between a box and a sphere. */
    CTEST_BOX_WITH_PLANE            = CLASS_TYPE_BOXCSHAPE | CLASS_TYPE_PLANECSHAPE << 8, /**< Collision between a box and a plane. */
    CTEST_BOX_WITH_CAPSULE          = CLASS_TYPE_BOXCSHAPE | CLASS_TYPE_CAPSULECSHAPE << 8, /**< Collision between a box and a capsule. */
    CTEST_BOX_WITH_MESH             = CLASS_TYPE_BOXCSHAPE | CLASS_TYPE_MESHCSHAPE << 8, /**< Collision between a box and a mesh. */
    CTEST_BOX_WITH_RAY              = CLASS_TYPE_BOXCSHAPE | CLASS_TYPE_RAYCSHAPE << 8, /**< Collision between a box and a ray. */
    CTEST_SPHERE_WITH_SPHERE        = CLASS_TYPE_SPHERECSHAPE | CLASS_TYPE_SPHERECSHAPE << 8, /**< Collision between two sphere shapes. */
    CTEST_SPHERE_WITH_PLANE         = CLASS_TYPE_SPHERECSHAPE | CLASS_TYPE_PLANECSHAPE << 8, /**< Collision between a sphere and a plane. */
    CTEST_SPHERE_WITH_CAPSULE       = CLASS_TYPE_SPHERECSHAPE | CLASS_TYPE_CAPSULECSHAPE << 8, /**< Collision between a sphere and a capsule. */
    CTEST_SPHERE_WITH_MESH          = CLASS_TYPE_SPHERECSHAPE | CLASS_TYPE_MESHCSHAPE << 8, /**< Collision between a sphere and a mesh. */
    CTEST_SPHERE_WITH_RAY           = CLASS_TYPE_SPHERECSHAPE | CLASS_TYPE_RAYCSHAPE << 8, /**< Collision between a sphere and a ray. */
    CTEST_PLANE_WITH_PLANE          = CLASS_TYPE_PLANECSHAPE | CLASS_TYPE_PLANECSHAPE << 8, /**< Collision between two plane shapes. */
    CTEST_PLANE_WITH_CAPSULE        = CLASS_TYPE_PLANECSHAPE | CLASS_TYPE_CAPSULECSHAPE << 8, /**< Collision between a plane and a capsule. */
    CTEST_PLANE_WITH_MESH           = CLASS_TYPE_PLANECSHAPE | CLASS_TYPE_MESHCSHAPE << 8, /**< Collision between a plane and a mesh. */
    CTEST_PLANE_WITH_RAY            = CLASS_TYPE_PLANECSHAPE | CLASS_TYPE_RAYCSHAPE << 8, /**< Collision between a plane and a ray. */
    CTEST_CAPSULE_WITH_CAPSULE      = CLASS_TYPE_CAPSULECSHAPE | CLASS_TYPE_CAPSULECSHAPE << 8, /**< Collision between two capsule shapes. */
    CTEST_CAPSULE_WITH_MESH         = CLASS_TYPE_CAPSULECSHAPE | CLASS_TYPE_MESHCSHAPE << 8, /**< Collision between a capsule and a mesh. */
    CTEST_CAPSULE_WITH_RAY          = CLASS_TYPE_CAPSULECSHAPE | CLASS_TYPE_RAYCSHAPE << 8, /**< Collision between a capsule and a ray. */
    CTEST_MESH_WITH_MESH            = CLASS_TYPE_MESHCSHAPE | CLASS_TYPE_MESHCSHAPE << 8, /**< Collision between two mesh shapes. */
    CTEST_MESH_WITH_RAY             = CLASS_TYPE_MESHCSHAPE | CLASS_TYPE_RAYCSHAPE << 8, /**< Collision between a mesh and a ray. */
    CTEST_RAY_WITH_RAY              = CLASS_TYPE_RAYCSHAPE | CLASS_TYPE_RAYCSHAPE << 8, /**< Collision between two ray shapes. */
};




/**
 * @brief Get the collision code between two shapes.
 * 
 * @param shapeA Pointer to the first shape's Node structure.
 * @param shapeB Pointer to the second shape's Node structure.
 * @return unsigned int Collision code representing the type of collision.
 */
unsigned int get_collision_code(struct Node *shapeA, struct Node *shapeB);


/**
 * @brief Apply collision response between two shapes.
 * 
 * @param shapeA Pointer to the first shape's Node structure.
 * @param shapeB Pointer to the second shape's Node structure.
 * @param collisionNormal Vector representing the collision normal.
 * @param impactPoint Vector representing the impact point.
 * @param penetrationDepth Depth of penetration between the two shapes.
 */
void apply_collision(struct Node *shapeA, struct Node *shapeB, vec3 collisionNormal, vec3 impactPoint, float penetrationDepth);




/**
 * @brief Check for collision between two box shapes.
 * 
 * @param boxA Pointer to the first box shape node.
 * @param boxB Pointer to the second box shape node.
 * @return true if the boxes collide, false otherwise.
 */
bool check_collision_box_with_box(struct Node *boxB, struct Node *boxA);

/**
 * @brief Check for collision between a box and a sphere shape.
 * 
 * @param shapeA Pointer to the box shape node.
 * @param shapeB Pointer to the sphere shape node.
 * @return true if the box and sphere collide, false otherwise.
 */
bool check_collision_box_with_sphere(struct Node *shapeA, struct Node *shapeB);

/**
 * @brief Check for collision between a box and a plane shape.
 * 
 * @param shapeA Pointer to the box shape node.
 * @param shapeB Pointer to the plane shape node.
 * @return true if the box and plane collide, false otherwise.
 */
bool check_collision_box_with_plane(struct Node *shapeA, struct Node *shapeB);

/**
 * @brief Check for collision between a box and a capsule shape.
 * 
 * @param shapeA Pointer to the box shape node.
 * @param shapeB Pointer to the capsule shape node.
 * @return true if the box and capsule collide, false otherwise.
 */
bool check_collision_box_with_capsule(struct Node *shapeA, struct Node *shapeB);

/**
 * @brief Check for collision between a box and a mesh shape.
 * 
 * @param shapeA Pointer to the box shape node.
 * @param shapeB Pointer to the mesh shape node.
 * @return true if the box and mesh collide, false otherwise.
 */
bool check_collision_box_with_mesh(struct Node *shapeA, struct Node *shapeB);

/**
 * @brief Check for collision between a box and a ray shape.
 * 
 * @param shapeA Pointer to the box shape node.
 * @param shapeB Pointer to the ray shape node.
 * @return true if the box and ray collide, false otherwise.
 */
bool check_collision_box_with_ray(struct Node *shapeA, struct Node *shapeB);

/**
 * @brief Check for collision between two sphere shapes.
 * 
 * @param shapeA Pointer to the first sphere shape node.
 * @param shapeB Pointer to the second sphere shape node.
 * @return true if the spheres collide, false otherwise.
 */
bool check_collision_sphere_with_sphere(struct Node *shapeA, struct Node *shapeB);

/**
 * @brief Check for collision between a sphere and a plane shape.
 * 
 * @param shapeA Pointer to the sphere shape node.
 * @param shapeB Pointer to the plane shape node.
 * @return true if the sphere and plane collide, false otherwise.
 */
bool check_collision_sphere_with_plane(struct Node *shapeA, struct Node *shapeB);

/**
 * @brief Check for collision between a sphere and a capsule shape.
 * 
 * @param shapeA Pointer to the sphere shape node.
 * @param shapeB Pointer to the capsule shape node.
 * @return true if the sphere and capsule collide, false otherwise.
 */
bool check_collision_sphere_with_capsule(struct Node *shapeA, struct Node *shapeB);

/**
 * @brief Check for collision between a sphere and a mesh shape.
 * 
 * @param shapeA Pointer to the sphere shape node.
 * @param shapeB Pointer to the mesh shape node.
 * @return true if the sphere and mesh collide, false otherwise.
 */
bool check_collision_sphere_with_mesh(struct Node *shapeA, struct Node *shapeB);

/**
 * @brief Check for collision between a sphere and a ray shape.
 * 
 * @param shapeA Pointer to the sphere shape node.
 * @param shapeB Pointer to the ray shape node.
 * @return true if the sphere and ray collide, false otherwise.
 */
bool check_collision_sphere_with_ray(struct Node *shapeA, struct Node *shapeB);

/**
 * @brief Check for collision between two plane shapes.
 * 
 * @param shapeA Pointer to the first plane shape node.
 * @param shapeB Pointer to the second plane shape node.
 * @return true if the planes collide, false otherwise.
 */
bool check_collision_plane_with_plane(struct Node *shapeA, struct Node *shapeB);

/**
 * @brief Check for collision between a plane and a capsule shape.
 * 
 * @param shapeA Pointer to the plane shape node.
 * @param shapeB Pointer to the capsule shape node.
 * @return true if the plane and capsule collide, false otherwise.
 */
bool check_collision_plane_with_capsule(struct Node *shapeA, struct Node *shapeB);

/**
 * @brief Check for collision between a plane and a mesh shape.
 * 
 * @param shapeA Pointer to the plane shape node.
 * @param shapeB Pointer to the mesh shape node.
 * @return true if the plane and mesh collide, false otherwise.
 */
bool check_collision_plane_with_mesh(struct Node *shapeA, struct Node *shapeB);

/**
 * @brief Check for collision between a plane and a ray shape.
 * 
 * @param shapeA Pointer to the plane shape node.
 * @param shapeB Pointer to the ray shape node.
 * @return true if the plane and ray collide, false otherwise.
 */
bool check_collision_plane_with_ray(struct Node *shapeA, struct Node *shapeB);

/**
 * @brief Check for collision between two capsule shapes.
 * 
 * @param shapeA Pointer to the first capsule shape node.
 * @param shapeB Pointer to the second capsule shape node.
 * @return true if the capsules collide, false otherwise.
 */
bool check_collision_capsule_with_capsule(struct Node *shapeA, struct Node *shapeB);

/**
 * @brief Check for collision between a capsule and a mesh shape.
 * 
 * @param shapeA Pointer to the capsule shape node.
 * @param shapeB Pointer to the mesh shape node.
 * @return true if the capsule and mesh collide, false otherwise.
 */
bool check_collision_capsule_with_mesh(struct Node *shapeA, struct Node *shapeB);

/**
 * @brief Check for collision between a capsule and a ray shape.
 * 
 * @param shapeA Pointer to the capsule shape node.
 * @param shapeB Pointer to the ray shape node.
 * @return true if the capsule and ray collide, false otherwise.
 */
bool check_collision_capsule_with_ray(struct Node *shapeA, struct Node *shapeB);

/**
 * @brief Check for collision between two mesh shapes.
 * 
 * @param shapeA Pointer to the first mesh shape node.
 * @param shapeB Pointer to the second mesh shape node.
 * @return true if the meshes collide, false otherwise.
 */
bool check_collision_mesh_with_mesh(struct Node *shapeA, struct Node *shapeB);

/**
 * @brief Check for collision between a mesh and a ray shape.
 * 
 * @param shapeA Pointer to the mesh shape node.
 * @param shapeB Pointer to the ray shape node.
 * @return true if the mesh and ray collide, false otherwise.
 */
bool check_collision_mesh_with_ray(struct Node *shapeA, struct Node *shapeB);

/**
 * @brief Check for collision between two ray shapes.
 * 
 * @param shapeA Pointer to the first ray shape node.
 * @param shapeB Pointer to the second ray shape node.
 * @return true if the rays collide, false otherwise.
 */
bool check_collision_ray_with_ray(struct Node *shapeA, struct Node *shapeB);

/** @} */