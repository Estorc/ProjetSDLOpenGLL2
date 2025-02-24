#define SCREEN_WIDTH   800
#define SCREEN_HEIGHT  600


#ifndef WINDOW_H
#define WINDOW_H

/**
 * @defgroup WindowManagement Window Management
 * @brief Functions and structures for managing the SDL window and OpenGL context.
 * @{
 */

/**
 * @brief Structure representing the SDL window and its associated OpenGL context.
 * 
 * This structure encapsulates the SDL window and its related properties, including
 * the OpenGL context, surfaces for rendering, and timing information.
 */
typedef struct Window {
    SDL_Window *sdl_window; /**< Pointer to the SDL window. */
    SDL_Surface *surface; /**< Main rendering surface. */
    SDL_Surface *ui_surface; /**< Surface for UI rendering. */
    TextureMap ui_texture; /**< Texture map for UI elements. */
    SDL_GLContext opengl_ctx; /**< OpenGL context associated with the window. */
    float startTime; /**< Time when the window was created or initialized. */
    float time; /**< Current time since the window was created or initialized. */
    float lastTime; /**< Time of the last frame update. */
    vec4 fadeColor; /**< Color used for fading the screen. */
    bool resized; /**< Flag indicating if the window has been resized. */
} Window;

struct WorldShaders;
struct Node;
struct Camera;
struct DepthMap;

/**
 * @brief Creates a new SDL window with an OpenGL context.
 * 
 * @param title The title of the window.
 * @param x The x-coordinate of the window's position on the screen.
 * @param y The y-coordinate of the window's position on the screen.
 * @param width The width of the window in pixels.
 * @param height The height of the window in pixels.
 * @param flags Flags to configure the window's behavior (e.g., fullscreen, resizable).
 * @param window Pointer to the Window structure to be initialized.
 * @return Returns 0 on success, or -1 on failure.
 * 
 * This function initializes the SDL2 library and creates a window with an OpenGL 
 * context. It sets various OpenGL attributes, including enabling double-buffering 
 * and depth testing. The window's start time and initial timing values are also 
 * initialized. If window creation or surface retrieval fails, an error message 
 * is printed and -1 is returned.
 */
s8 create_window(char *title, s32 x, s32 y, s32 width, s32 height, u32 flags, Window *window);

/**
 * @brief Refreshes the window resolution.
 */
void refresh_resolution();

/**
 * @brief Updates the window by rendering the scene and swapping the Game.buffers->
 * 
 * @param window Pointer to the Window structure containing 
 *               the window's state and timing information.
 * @param viewportNode Pointer to the Node structure representing 
 *                     the viewport to be rendered.
 * @param c Pointer to the Camera structure used for rendering 
 *          the scene.
 * @param shaders Array of Shader structures used for rendering.
 * @param depthMap Pointer to the DepthMap structure that handles 
 *                 depth information for rendering.
 * @return This function does not return a value.
 * 
 * This function calculates the time elapsed since the last update, 
 * updates the window's state accordingly, and calls the draw_screen function 
 * to render the viewport's content. It also enables relative mouse mode 
 * for capturing mouse movements. Finally, it swaps the OpenGL buffers 
 * to display the rendered content on the window.
 */
void update_window(Window *window, struct Node *scene, struct Camera *c, struct WorldShaders *shaders, struct DepthMap *depthMap, struct MSAA *msaa, struct Mesh *screenPlane);

/**
 * @brief Refreshes the UI of the window.
 * 
 * @param window Pointer to the Window structure.
 */
void refresh_ui(Window *window);

/**
 * @brief Frees the resources associated with the window.
 * 
 * @param window Pointer to the Window structure to be freed.
 */
void free_window(Window *window);

/** @} */ // end of WindowManagement group

#endif