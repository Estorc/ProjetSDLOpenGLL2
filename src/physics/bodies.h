#include "../math/math_util.h"
#include "../io/model.h"

#ifndef BODIES_H
#define BODIES_H

/**
 * @defgroup BodiesGroup Bodies
 * @brief Represents a node in the physics simulation.
 * 
 * A node is a generic object that can be used to represent different types of 
 * bodies in the physics simulation, such as static bodies, rigid bodies, and 
 * kinematic bodies.
 * 
 * @{
 */

struct Node;

/**
 * @brief Represents a static body in the physics simulation.
 * 
 * A static body does not move and is used for objects like walls or floors.
 */
typedef struct StaticBody {
    struct Node **collisionsShapes; /**< Array of collision shapes associated with the static body. */
    u8 length; /**< Number of collision shapes. */
} StaticBody;

/**
 * @brief Represents a rigid body in the physics simulation.
 * 
 * A rigid body can move and is affected by forces and collisions.
 */
typedef struct RigidBody {
    struct Node **collisionsShapes; /**< Array of collision shapes associated with the rigid body. */
    vec3 velocity; /**< Current velocity of the rigid body. */
    vec3 gravity; /**< Gravity affecting the rigid body. */
    vec3 angularVelocity; /**< Angular velocity of the rigid body. */
    vec3 centerOfMass; /**< Center of mass of the rigid body. */
    float mass; /**< Mass of the rigid body. */
    float friction; /**< Friction coefficient of the rigid body. */
    u8 length; /**< Number of collision shapes. */
} RigidBody;

/**
 * @brief Represents a kinematic body in the physics simulation.
 * 
 * A kinematic body is controlled by the user and is not affected by forces.
 */
typedef struct KinematicBody {
    struct Node **collisionsShapes; /**< Array of collision shapes associated with the kinematic body. */
    vec3 velocity; /**< Current velocity of the kinematic body. */
    u8 length; /**< Number of collision shapes. */
} KinematicBody;

/**
 * @brief Represents a plane collision shape.
 * 
 * A plane collision shape is an infinite plane used for collision detection.
 */
typedef struct PlaneCollisionShape {
    u8 UNUSED; /**< Placeholder for future use. */
} PlaneCollisionShape;

/**
 * @brief Represents a box collision shape.
 * 
 * A box collision shape is a rectangular prism used for collision detection.
 */
typedef struct BoxCollisionShape {
    u8 UNUSED; /**< Placeholder for future use. */
} BoxCollisionShape;

/**
 * @brief Represents a sphere collision shape.
 * 
 * A sphere collision shape is a sphere used for collision detection.
 */
typedef struct SphereCollisionShape {
    u8 UNUSED; /**< Placeholder for future use. */
} SphereCollisionShape;

/**
 * @brief Represents a mesh collision shape.
 * 
 * A mesh collision shape is a complex shape defined by a mesh of vertices.
 */
typedef struct MeshCollisionShape {
    Vertex (*facesVertex)[3]; /**< Array of faces, each defined by three vertices. */
    u32 numFaces; /**< Number of faces in the mesh. */
} MeshCollisionShape;

/**
 * @brief Represents a capsule collision shape.
 * 
 * A capsule collision shape is a cylinder with hemispherical ends used for collision detection.
 */
typedef struct CapsuleCollisionShape {
    u8 UNUSED; /**< Placeholder for future use. */
} CapsuleCollisionShape;

/**
 * @brief Represents a ray collision shape.
 * 
 * A ray collision shape is a line segment used for collision detection.
 */
typedef struct RayCollisionShape {
    u8 UNUSED; /**< Placeholder for future use. */
} RayCollisionShape;

/**
 * @brief Represents a buffer for collision shapes.
 * 
 * A collision buffer stores collision shapes and manages their indices.
 */
typedef struct CollisionBuffer {
    struct Node **collisionsShapes; /**< Array of collision shapes in the buffer. */
    u8 length; /**< Number of collision shapes in the buffer. */
    u8 index; /**< Current index in the buffer. */
} CollisionBuffer;

// All the bodies listed bellow have shared attributes. It allows the compiler to get an attribute from the void* pointer.
#define GET_FROM_BODY_NODE(node, attribute, dest) \
switch (node->type) {\
    case CLASS_TYPE_STATICBODY:              dest = &((StaticBody *) node->object)->attribute;                  break;\
    case CLASS_TYPE_RIGIDBODY:               dest = &((RigidBody *) node->object)->attribute;                   break;\
    case CLASS_TYPE_KINEMATICBODY:           dest = &((KinematicBody *) node->object)->attribute;               break;\
    default:                            dest = 0;                                                         break;\
};

 /**
 * @brief Adds a child shape to the specified node.
 * 
 * @param node The parent node to which the child shape will be added.
 * @param child The child shape to be added to the node.
 */
 
void add_shape(struct Node *node, struct Node *child);


/**
 * @brief Adds a child shape to the specified node and reallocates memory if necessary.
 * 
 * @param node The parent node to which the child shape will be added.
 * @param child The child shape to be added to the node.
 */

void add_shape_and_realloc(struct Node *node, struct Node *child);


/**
 * @brief Removes a child shape from the specified node.
 * 
 * @param node The parent node from which the child shape will be removed.
 * @param child The child shape to be removed from the node.
 */

void remove_shape(struct Node *node, struct Node *child);


/**
 * @brief Removes a child shape from the specified node and reallocates memory if necessary.
 * 
 * @param node The parent node from which the child shape will be removed.
 * @param child The child shape to be removed from the node.
 */

void remove_shape_and_realloc(struct Node *node, struct Node *child);


/**
 * @brief Removes a child shape from the specified node and frees the memory allocated for the child shape.
 * 
 * @param node The parent node from which the child shape will be removed.
 * @param child The child shape to be removed and freed.
 */

void remove_shape_and_free(struct Node *node, struct Node *child);


/**
 * @brief Removes a child shape from the specified node, frees the memory allocated for the child shape, and reallocates memory if necessary.
 * 
 * @param node The parent node from which the child shape will be removed.
 * @param child The child shape to be removed, freed, and reallocated.
 */
void remove_shape_and_free_and_realloc(struct Node *node, struct Node *child);

/** @} */

#endif