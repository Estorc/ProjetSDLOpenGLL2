#ifndef RENDER_H
#define RENDER_H

struct Window;
struct DepthMap;
struct Camera;
struct Node;
struct MSAA;
struct Mesh;

/**
 * @defgroup RenderFunctions Render Functions
 * @brief Functions for configuring shaders, rendering scenes, and drawing shadow maps.
 * @{
 */


/**
 * @struct WorldShaders
 * @brief A structure to hold various shaders used in the rendering process.
 *
 * This structure contains different shaders that are used for rendering
 * different aspects of the world, such as the main render, depth, screen,
 * skybox, and GUI.
 */
typedef struct WorldShaders {
    Shader render; /**< Main render shader */
    Shader depth; /**< Depth shader */
    Shader screen; /**< Screen shader */
    Shader skybox; /**< Skybox shader */
    Shader gui; /**< GUI shader */
} WorldShaders;


/**
 * @brief Renders the scene using the specified shader.
 *
 * This function renders the scene graph starting from the given node using the active shader.
 * It applies the model matrix transformations and uses the provided camera and world shaders
 * to render the scene.
 *
 * @param window Pointer to the Window structure.
 * @param node Pointer to the current Node in the scene graph.
 * @param c Pointer to the Camera structure.
 * @param modelMatrix The model matrix for transforming the node.
 * @param activeShader The shader program to use for rendering.
 * @param shaders Pointer to the WorldShaders structure containing shader programs.
 * @param viewportWidth The width of the viewport.
 * @param viewportHeight The height of the viewport.
 */
void render_scene(struct Window *window, struct Node *node, struct Camera *c, mat4 modelMatrix, Shader activeShader, struct WorldShaders *shaders, int viewportWidth, int viewportHeight);

/**
 * @brief Draws the shadow map for the scene.
 *
 * This function generates the shadow map by rendering the scene from the light's perspective.
 * It uses the provided window, scene graph root node, camera, world shaders, and depth map
 * to create the shadow map.
 *
 * @param window Pointer to the Window structure.
 * @param root Pointer to the root Node of the scene graph.
 * @param c Pointer to the Camera structure.
 * @param shaders Pointer to the WorldShaders structure containing shader programs.
 * @param depthMap Pointer to the DepthMap structure for storing the shadow map.
 */
void draw_shadow_map(struct Window *window, struct Node *root, struct Camera *c, struct WorldShaders *shaders, struct DepthMap *depthMap);

/**
 * @brief Draws the entire scene.
 *
 * This function renders the entire scene graph starting from the root node. It uses the provided
 * window, camera, world shaders, and depth map to render the scene with shadows and other effects.
 *
 * @param window Pointer to the Window structure.
 * @param root Pointer to the root Node of the scene graph.
 * @param c Pointer to the Camera structure.
 * @param shaders Pointer to the WorldShaders structure containing shader programs.
 * @param depthMap Pointer to the DepthMap structure for shadow mapping.
 */
void draw_scene(struct Window *window, struct Node *root, struct Camera *c, struct WorldShaders *shaders, struct DepthMap *depthMap);

/**
 * @brief Draws the final screen output.
 *
 * This function renders the final output to the screen. It uses the provided window, scene graph,
 * camera, world shaders, depth map, MSAA settings, and screen plane mesh to draw the final image
 * on the screen.
 *
 * @param window Pointer to the Window structure.
 * @param scene Pointer to the root Node of the scene graph.
 * @param c Pointer to the Camera structure.
 * @param shaders Pointer to the WorldShaders structure containing shader programs.
 * @param depthMap Pointer to the DepthMap structure for shadow mapping.
 * @param msaa Pointer to the MSAA structure for multi-sample anti-aliasing.
 * @param screenPlane Pointer to the Mesh structure representing the screen plane.
 */
void draw_screen(struct Window *window, struct Node *scene, struct Camera *c, WorldShaders *shaders, struct DepthMap *depthMap, struct MSAA *msaa, struct Mesh *screenPlane);

/** @} */ // end of RenderFunctions group
#endif