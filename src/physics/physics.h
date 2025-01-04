#include "../io/input.h"
#include "../render/lighting.h"

struct Input;
struct Window;
struct Node;

#define DEBUG_AREA

/**
 * @defgroup Physics Physics
 * @brief Functions and structures for physics and collision detection.
 * @{
 */

/**
 * @struct DistanceNode
 * @brief Represents a node and its distance.
 * 
 * This structure holds a reference to a node and its associated distance.
 * It is used for sorting and managing nodes based on their distances.
 * 
 * @var DistanceNode::node
 * Pointer to the node.
 * 
 * @var DistanceNode::distance
 * Distance associated with the node.
 */

typedef struct DistanceNode {
    struct Node *node;
    float distance;
} DistanceNode;

/**
 * @struct Area
 * @brief Represents an area with collision shapes and nodes.
 * 
 * This structure holds information about an area, including its collision shapes,
 * collected nodes, and sorted nodes.
 * 
 * @var Area::collisionsShapes
 * Array of pointers to collision shapes.
 * 
 * @var Area::length
 * Number of collision shapes.
 * 
 * @var Area::collectedNodes
 * Array of collected nodes with distances.
 * 
 * @var Area::collectedLength
 * Number of collected nodes.
 * 
 * @var Area::sortedNodes
 * Array of sorted nodes with distances.
 * 
 * @var Area::sortedLength
 * Number of sorted nodes.
 */

typedef struct Area {
    struct Node **collisionsShapes;
    u8 length;
    struct DistanceNode *collectedNodes;
    u8 collectedLength;
    struct DistanceNode *sortedNodes;
    u8 sortedLength;
} Area;

/**
 * @brief Get the velocity norm of a node.
 * 
 * @param node Pointer to the node.
 * @return The velocity norm of the node.
 */
float get_velocity_norm(struct Node *node);

/**
 * @brief Get the velocity of a node.
 * 
 * @param node Pointer to the node.
 * @param velocity Output vector to store the velocity.
 */
void get_velocity(struct Node *node, vec3 velocity);

/**
 * @brief Get the mass of a node.
 * 
 * @param node Pointer to the node.
 * @param mass Output pointer to store the mass.
 */
void get_mass(struct Node *node, float *mass);

/**
 * @brief Get the center of mass of a node.
 * 
 * @param node Pointer to the node.
 * @param com Output vector to store the center of mass.
 */
void get_center_of_mass(struct Node *node, vec3 com);

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