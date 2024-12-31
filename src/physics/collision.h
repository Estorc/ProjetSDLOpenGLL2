#pragma once


enum CollisionsCode {
    CTEST_BOX_WITH_BOX              = CLASS_TYPE_BOXCSHAPE | CLASS_TYPE_BOXCSHAPE << 8,
    CTEST_BOX_WITH_SPHERE           = CLASS_TYPE_BOXCSHAPE | CLASS_TYPE_SPHERECSHAPE << 8,
    CTEST_BOX_WITH_PLANE            = CLASS_TYPE_BOXCSHAPE | CLASS_TYPE_PLANECSHAPE << 8,
    CTEST_BOX_WITH_CAPSULE          = CLASS_TYPE_BOXCSHAPE | CLASS_TYPE_CAPSULECSHAPE << 8,
    CTEST_BOX_WITH_MESH             = CLASS_TYPE_BOXCSHAPE | CLASS_TYPE_MESHCSHAPE << 8,
    CTEST_BOX_WITH_RAY              = CLASS_TYPE_BOXCSHAPE | CLASS_TYPE_RAYCSHAPE << 8,
    CTEST_SPHERE_WITH_SPHERE        = CLASS_TYPE_SPHERECSHAPE | CLASS_TYPE_SPHERECSHAPE << 8,
    CTEST_SPHERE_WITH_PLANE         = CLASS_TYPE_SPHERECSHAPE | CLASS_TYPE_PLANECSHAPE << 8,
    CTEST_SPHERE_WITH_CAPSULE       = CLASS_TYPE_SPHERECSHAPE | CLASS_TYPE_CAPSULECSHAPE << 8,
    CTEST_SPHERE_WITH_MESH          = CLASS_TYPE_SPHERECSHAPE | CLASS_TYPE_MESHCSHAPE << 8,
    CTEST_SPHERE_WITH_RAY           = CLASS_TYPE_SPHERECSHAPE | CLASS_TYPE_RAYCSHAPE << 8,
    CTEST_PLANE_WITH_PLANE          = CLASS_TYPE_PLANECSHAPE | CLASS_TYPE_PLANECSHAPE << 8,
    CTEST_PLANE_WITH_CAPSULE        = CLASS_TYPE_PLANECSHAPE | CLASS_TYPE_CAPSULECSHAPE << 8,
    CTEST_PLANE_WITH_MESH           = CLASS_TYPE_PLANECSHAPE | CLASS_TYPE_MESHCSHAPE << 8,
    CTEST_PLANE_WITH_RAY            = CLASS_TYPE_PLANECSHAPE | CLASS_TYPE_RAYCSHAPE << 8,
    CTEST_CAPSULE_WITH_CAPSULE      = CLASS_TYPE_CAPSULECSHAPE | CLASS_TYPE_CAPSULECSHAPE << 8,
    CTEST_CAPSULE_WITH_MESH         = CLASS_TYPE_CAPSULECSHAPE | CLASS_TYPE_MESHCSHAPE << 8,
    CTEST_CAPSULE_WITH_RAY          = CLASS_TYPE_CAPSULECSHAPE | CLASS_TYPE_RAYCSHAPE << 8,
    CTEST_MESH_WITH_MESH            = CLASS_TYPE_MESHCSHAPE | CLASS_TYPE_MESHCSHAPE << 8,
    CTEST_MESH_WITH_RAY             = CLASS_TYPE_MESHCSHAPE | CLASS_TYPE_RAYCSHAPE << 8,
    CTEST_RAY_WITH_RAY              = CLASS_TYPE_RAYCSHAPE | CLASS_TYPE_RAYCSHAPE << 8,
};

bool check_collision_box_with_box(struct Node *boxB, struct Node *boxA);
bool check_collision_box_with_sphere(struct Node *shapeA, struct Node *shapeB);
bool check_collision_box_with_plane(struct Node *shapeA, struct Node *shapeB);
bool check_collision_box_with_capsule(struct Node *shapeA, struct Node *shapeB);
bool check_collision_box_with_mesh(struct Node *shapeA, struct Node *shapeB);
bool check_collision_box_with_ray(struct Node *shapeA, struct Node *shapeB);
bool check_collision_sphere_with_sphere(struct Node *shapeA, struct Node *shapeB);
bool check_collision_sphere_with_plane(struct Node *shapeA, struct Node *shapeB);
bool check_collision_sphere_with_capsule(struct Node *shapeA, struct Node *shapeB);
bool check_collision_sphere_with_mesh(struct Node *shapeA, struct Node *shapeB);
bool check_collision_sphere_with_ray(struct Node *shapeA, struct Node *shapeB);
bool check_collision_plane_with_plane(struct Node *shapeA, struct Node *shapeB);
bool check_collision_plane_with_capsule(struct Node *shapeA, struct Node *shapeB);
bool check_collision_plane_with_mesh(struct Node *shapeA, struct Node *shapeB);
bool check_collision_plane_with_ray(struct Node *shapeA, struct Node *shapeB);
bool check_collision_capsule_with_capsule(struct Node *shapeA, struct Node *shapeB);
bool check_collision_capsule_with_mesh(struct Node *shapeA, struct Node *shapeB);
bool check_collision_capsule_with_ray(struct Node *shapeA, struct Node *shapeB);
bool check_collision_mesh_with_mesh(struct Node *shapeA, struct Node *shapeB);
bool check_collision_mesh_with_ray(struct Node *shapeA, struct Node *shapeB);
bool check_collision_ray_with_ray(struct Node *shapeA, struct Node *shapeB);