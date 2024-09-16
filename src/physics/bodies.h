#ifndef BODIES_H
#define BODIES_H

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

typedef struct CollisionBuffer {
    struct Node **collisionsShapes;
    u8 length;
} CollisionBuffer;

#define IS_NODE_BODY(x) (x > _M_NODE_BODIES__START__ && x < _M_NODE_BODIES__END__)
#define IS_NODE_CSHAPE(x) (x > _M_NODE_CSHAPES__START__ && x < _M_NODE_CSHAPES__END__)

// All the bodies listed bellow have shared attributes. It allows the compiler to get an attribute from the void* pointer.
#define GET_FROM_BODY_NODE(node, attribute, dest) \
switch (node->type) {\
    case NODE_STATIC_BODY:              dest = &((StaticBody *) node->object)->attribute;                  break;\
    case NODE_RIGID_BODY:               dest = &((RigidBody *) node->object)->attribute;                   break;\
    case NODE_KINEMATIC_BODY:           dest = &((KinematicBody *) node->object)->attribute;               break;\
    default:                            dest = 0;                                                         break;\
};

#endif

void add_shape(struct Node *node, struct Node *child);
void add_shape_and_realloc(struct Node *node, struct Node *child);
void remove_shape(struct Node *node, struct Node *child);
void remove_shape_and_realloc(struct Node *node, struct Node *child);
void remove_shape_and_free(struct Node *node, struct Node *child);
void remove_shape_and_free_and_realloc(struct Node *node, struct Node *child);