#include "../math/math_util.h"
#include "../io/model.h"

#ifndef BODIES_H
#define BODIES_H

struct Node;

typedef struct StaticBody {
    struct Node **collisionsShapes;
    u8 length;
} StaticBody;

typedef struct RigidBody {
    struct Node **collisionsShapes;
    vec3 velocity;
    vec3 gravity;
    vec3 angularVelocity;
    vec3 centerOfMass;
    float mass;
    float friction;
    u8 length;
} RigidBody;

typedef struct KinematicBody {
    struct Node **collisionsShapes;
    vec3 velocity;
    u8 length;
} KinematicBody;

typedef struct PlaneCollisionShape {
    u8 UNUSED;
} PlaneCollisionShape;

typedef struct BoxCollisionShape {
    u8 UNUSED;
} BoxCollisionShape;

typedef struct SphereCollisionShape {
    u8 UNUSED;
} SphereCollisionShape;

typedef struct MeshCollisionShape {
    Vertex (*facesVertex)[3];
    u32 numFaces;
} MeshCollisionShape;

typedef struct CapsuleCollisionShape {
    u8 UNUSED;
} CapsuleCollisionShape;

typedef struct RayCollisionShape {
    u8 UNUSED;
} RayCollisionShape;

typedef struct CollisionBuffer {
    struct Node **collisionsShapes;
    u8 length;
    u8 index;
} CollisionBuffer;

// All the bodies listed bellow have shared attributes. It allows the compiler to get an attribute from the void* pointer.
#define GET_FROM_BODY_NODE(node, attribute, dest) \
switch (node->type) {\
    case CLASS_TYPE_STATICBODY:              dest = &((StaticBody *) node->object)->attribute;                  break;\
    case CLASS_TYPE_RIGIDBODY:               dest = &((RigidBody *) node->object)->attribute;                   break;\
    case CLASS_TYPE_KINEMATICBODY:           dest = &((KinematicBody *) node->object)->attribute;               break;\
    default:                            dest = 0;                                                         break;\
};

#endif

void add_shape(struct Node *node, struct Node *child);
void add_shape_and_realloc(struct Node *node, struct Node *child);
void remove_shape(struct Node *node, struct Node *child);
void remove_shape_and_realloc(struct Node *node, struct Node *child);
void remove_shape_and_free(struct Node *node, struct Node *child);
void remove_shape_and_free_and_realloc(struct Node *node, struct Node *child);