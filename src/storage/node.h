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
    Script scripts[SCRIPTS_COUNT]; /**< The scripts associated with the tree. */
    u8 scriptIndex; /**< The index of the current script. */
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
    BehaviorParameter *params; /**< The parameters associated with the behavior. */
    u8 params_count; /**< The number of parameters associated with the behavior. */

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
    NODE_UNUSED3            = 1 << 4, // 0001 0000
    NODE_UNUSED4            = 1 << 5, // 0010 0000
    NODE_UNUSED5            = 1 << 6, // 0100 0000
    NODE_EDITOR_FLAG        = 1 << 7, // 1000 0000
};


#define NODE_ACTIVE_AND_VISIBLE NODE_ACTIVE | NODE_VISIBLE // 0000 0011
#define NODE_DEFAULT_FLAGS NODE_ACTIVE_AND_VISIBLE // 0000 0011



 /**
 * @brief Changes the current scene.
 * 
 * This function is responsible for transitioning from the current scene to a new one.
 * It handles any necessary cleanup and setup required for the scene change.
 */
void change_scene();

/**
 * @brief Adds a child node to a parent node.
 * 
 * This function adds a child node to the specified parent node, establishing a parent-child
 * relationship in the scene graph.
 * 
 * @param node The parent node to which the child will be added.
 * @param child The child node to be added.
 */
void add_child(Node *node, Node *child);

/**
 * @brief Adds a child node to a parent node and reallocates memory if necessary.
 * 
 * This function adds a child node to the specified parent node and reallocates memory
 * for the parent's children array if needed to accommodate the new child.
 * 
 * @param node The parent node to which the child will be added.
 * @param child The child node to be added.
 */
void add_child_and_realloc(Node *node, Node *child);

/**
 * @brief Removes a child node from a parent node.
 * 
 * This function removes a child node from the specified parent node, breaking the
 * parent-child relationship in the scene graph.
 * 
 * @param node The parent node from which the child will be removed.
 * @param child The child node to be removed.
 */
void remove_child(Node *node, Node *child);

/**
 * @brief Removes a child node from a parent node and reallocates memory if necessary.
 * 
 * This function removes a child node from the specified parent node and reallocates memory
 * for the parent's children array if needed to optimize memory usage.
 * 
 * @param node The parent node from which the child will be removed.
 * @param child The child node to be removed.
 */
void remove_child_and_realloc(Node *node, Node *child);

/**
 * @brief Removes a child node from a parent node and frees the memory of the child node.
 * 
 * This function removes a child node from the specified parent node and frees the memory
 * allocated for the child node.
 * 
 * @param node The parent node from which the child will be removed.
 * @param child The child node to be removed and freed.
 */
void remove_child_and_free(Node *node, Node *child);

/**
 * @brief Removes a child node from a parent node, frees the memory of the child node, and reallocates memory if necessary.
 * 
 * This function removes a child node from the specified parent node, frees the memory
 * allocated for the child node, and reallocates memory for the parent's children array
 * if needed to optimize memory usage.
 * 
 * @param node The parent node from which the child will be removed.
 * @param child The child node to be removed and freed.
 */
void remove_child_and_free_and_realloc(Node *node, Node *child);

/**
 * @brief Renders a node using the specified model matrix and shader.
 * 
 * This function renders the specified node using the provided model matrix and active shader.
 * It utilizes the collection of shaders available in the world for rendering.
 * 
 * @param node The node to be rendered.
 * @param modelMatrix The model matrix to be used for rendering.
 * @param activeShader The shader to be used for rendering.
 * @param shaders The collection of shaders available in the world.
 */
void render_node(Node *node, mat4 modelMatrix, Shader activeShader, struct WorldShaders *shaders);

/**
 * @brief Renders a point light node using the specified model matrix.
 * 
 * This function renders the specified point light node using the provided model matrix.
 * 
 * @param node The point light node to be rendered.
 * @param modelMatrix The model matrix to be used for rendering.
 */
void render_point_light(Node *node, mat4 modelMatrix);

/**
 * @brief Renders a directional light node using the specified model matrix.
 * 
 * This function renders the specified directional light node using the provided model matrix.
 * 
 * @param node The directional light node to be rendered.
 * @param modelMatrix The model matrix to be used for rendering.
 */
void render_directional_light(Node *node, mat4 modelMatrix);

/**
 * @brief Frees the memory allocated for a node.
 * 
 * This function frees the memory allocated for the specified node, ensuring that all
 * resources associated with the node are properly released.
 * 
 * @param node The node to be freed.
 */
void free_node(Node *node);

/**
 * @brief Prints the details of a node at a specified level of indentation.
 * 
 * This function prints the details of the specified node, including its properties and
 * children, at the specified level of indentation for better readability.
 * 
 * @param node The node to be printed.
 * @param level The level of indentation for printing.
 */
void print_node(Node *node, int level);

/**
 * @brief The main tree structure used in the storage system.
 *
 * This external variable represents the primary tree structure that is
 * utilized for storing and managing nodes within the system.
 */
extern Tree mainNodeTree;

#endif