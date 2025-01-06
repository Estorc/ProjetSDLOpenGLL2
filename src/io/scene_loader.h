#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H
struct Node;
struct Camera;

/**
 * @defgroup SceneLoader Scene Loader
 * @brief Functions for loading nodes and scenes from files.
 * @{
 */

/**
 * @brief Loads a node from a file.
 *
 * This function reads a node from the given file and initializes it.
 * It also updates the provided camera and scripts as necessary.
 *
 * @param file The file to read the node from.
 * @param c A pointer to the camera to be updated.
 * @param scripts An array of scripts to be updated.
 * @param editor A pointer to the editor node.
 * @return A pointer to the loaded node.
 */
struct Node *load_node(FILE *file, struct Camera **c, Script *scripts, struct Node *editor);

/**
 * @brief Loads a scene from a file.
 *
 * This function reads a scene from the specified file path and initializes it.
 * It also updates the provided camera and scripts as necessary.
 *
 * @param path The path to the file to read the scene from.
 * @param c A pointer to the camera to be updated.
 * @param scripts An array of scripts to be updated.
 * @return A pointer to the loaded scene.
 */
struct Node *load_scene(char *path, struct Camera **c, Script *scripts);

/** @} */
#endif