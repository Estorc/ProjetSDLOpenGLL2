/**
 * @defgroup SceneFunctions Scene Management
 * @brief Functions for managing scenes.
 * @{
 */ 
 
 /**
 * @brief Changes the current scene.
 * 
 * This function is responsible for transitioning from the current scene to a new one.
 * It handles any necessary cleanup and setup required for the scene change.
 */
void change_scene();

/**
 * @brief Prepares to change the current scene.
 * 
 * This function prepares the scene change by pushing the necessary data to the call queue.
 * 
 * @param path The path to the new scene.
 */

void prepare_change_scene(const char *path);

/** @} */