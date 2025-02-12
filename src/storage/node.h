#include "../io/shader.h"
#include "../render/camera.h"
#include "../physics/bodies.h"
#include "../scripts/scripts.h"
#include "../render/framebuffer.h"

#ifndef NODE_H
#define NODE_H

struct PointLight;
struct DirectionalLight;
struct SpotLight;
struct Camera;
struct Frame;
struct StaticBody;
struct RigidBody;
struct KinematicBody;
struct PlaneCollisionShape;
struct BoxCollisionShape;
struct SphereCollisionShape;
struct Model;
struct TexturedMesh;
struct Mesh;
struct Viewport;
struct Filter;
struct WorldShaders;

/**
 * @defgroup NodeFunctions Node Data Structure
 * @brief Functions for managing and rendering nodes in the scene graph.
 * @{
 */



/**
 * @struct Tree
 * @brief Represents a tree structure containing nodes, a camera, MSAA settings, and scripts.
 */

typedef struct Tree {
    struct Node *root; /**< The root node of the tree structure. */
    struct Camera *camera; /**< The camera associated with the tree. */
    struct MSAA msaa; /**< The MSAA settings for the tree. */
    Script *scripts; /**< The scripts associated with the tree. */
} Tree;

/**
 * @struct Node
 * @brief Represents a node in the tree structure.
 */

typedef struct Node {
    void *object; /**< A pointer to an object associated with the node. */
    struct Node **children; /**< An array of child nodes. */
    struct Node *parent; /**< A pointer to the parent node. */
    u16 length; /**< The number of child nodes. */
    u8 type; /**< The type of the node. */
    u8 flags; /**< Flags indicating the state and properties of the node. */

    vec3 pos; /**< The position of the node. */
    vec3 rot; /**< The rotation of the node. */
    vec3 scale; /**< The scale of the node. */

    vec3 globalPos; /**< The global position of the node. */
    vec3 globalRot; /**< The global rotation of the node. */
    vec3 globalScale; /**< The global scale of the node. */

    Behavior *behavior; /**< The behavior associated with the node. */
    BehaviorAttribute *attribute; /**< The parameters associated with the behavior. */
    u8 attributes_count; /**< The number of parameters associated with the behavior. */

    Shader shader; /**< The shader associated with the node. */
} Node;

/**
 * @enum NodeFlags
 * @brief Flags indicating the state and properties of a node.
 */

enum NodeFlags {
    NODE_ACTIVE             = 1 << 0, // 0000 0001
    NODE_VISIBLE            = 1 << 1, // 0000 0010
    NODE_SCRIPT             = 1 << 2, // 0000 0100 
    NODE_LIGHT_ACTIVE       = 1 << 3, // 0000 1000
    NODE_INITIALIZED        = 1 << 4, // 0001 0000
    NODE_UNUSED4            = 1 << 5, // 0010 0000
    NODE_UNUSED5            = 1 << 6, // 0100 0000
    NODE_EDITOR_FLAG        = 1 << 7, // 1000 0000
};


#define NODE_ACTIVE_AND_VISIBLE NODE_ACTIVE | NODE_VISIBLE // 0000 0011
#define NODE_DEFAULT_FLAGS NODE_ACTIVE_AND_VISIBLE // 0000 0011



/**
 * @brief The main tree structure used in the storage system.
 *
 * This external variable represents the primary tree structure that is
 * utilized for storing and managing nodes within the system.
 */
extern Tree mainNodeTree;

#endif