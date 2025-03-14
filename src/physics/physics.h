#pragma once
#include "../io/input.h"
#include "../render/lighting.h"

struct Input;
struct Window;
struct Node;

/**
 * @defgroup Physics Physics
 * @brief Functions and structures for physics and collision detection.
 * @{
 */

/**
 * @struct CollectedNode
 * @brief Represents a node collected by an area.
 * 
 * This structure holds a reference to a node and associated informations.
 * It is used for sorting and managing nodes based on their distances.
 */
typedef struct CollectedNode {
    struct Node *node; /** < Pointer to the node. */
    float distance; /** < Distance associated with the node. */
    vec3 impactPoint; /** < Impact point associated with the node. */
} CollectedNode;

/**
 * @struct Area
 * @brief Represents an area with collision shapes and nodes.
 * 
 * This structure holds information about an area, including its collision shapes,
 * collected nodes, and sorted nodes.
 */
typedef struct Area {
    struct Node **collisionsShapes; /** < Array of collected nodes with distances. */
    u8 length; /** < Number of collision shapes. */
    struct CollectedNode *collectedNodes; /** < Array of collected nodes with distances. */
    u32 collectedLength; /** < Number of collected nodes. */
    u16 signal_id; /** < Signal ID for the area. */
} Area;


/**
 * @brief Apply body collision between two shapes.
 * 
 * @param shapeA Pointer to the first shape.
 * @param shapeB Pointer to the second shape.
 * @param collisionNormal Vector representing the collision normal.
 * @param angularNormal Vector representing the angular normal.
 * @param penetrationDepth Depth of penetration during collision.
 */
void apply_body_collision(struct Node *shapeA, struct Node *shapeB, vec3 collisionNormal, vec3 angularNormal, float penetrationDepth);

/**
 * @brief Check collisions for a shape.
 * 
 * @param shape Pointer to the shape.
 */
void check_collisions(struct Node *shape);

/**
 * @brief Update the script for a node.
 * 
 * @param node Pointer to the node.
 * @param pos Position vector.
 * @param rot Rotation vector.
 * @param scale Scale vector.
 * @param delta Time delta.
 * @param input Pointer to the input structure.
 * @param window Pointer to the window structure.
 */
void update_script(struct Node *node, vec3 pos, vec3 rot, vec3 scale, float delta, struct Input *input, struct Window *window);

/**
 * @brief Update the physics for a node.
 * 
 * @param node Pointer to the node.
 * @param pos Position vector.
 * @param rot Rotation vector.
 * @param scale Scale vector.
 * @param delta Time delta.
 * @param input Pointer to the input structure.
 * @param window Pointer to the window structure.
 * @param lightsCount Array of light counts.
 * @param active Boolean indicating if the node is active.
 */
void update_physics(struct Node *node, vec3 pos, vec3 rot, vec3 scale, float delta, struct Input *input, struct Window *window, u8 lightsCount[LIGHTS_COUNT], bool active);

/**
 * @brief Update the global position of a node.
 * 
 * @param node Pointer to the node.
 * @param pos Position vector.
 * @param rot Rotation vector.
 * @param scale Scale vector.
 */
void update_global_position(struct Node *node, vec3 pos, vec3 rot, vec3 scale);

/** @} */ // end of Physics group