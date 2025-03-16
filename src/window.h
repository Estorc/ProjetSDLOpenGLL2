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
 * @enum WindowFlags
 * @brief Enumeration for window flags.
 *
 * This enumeration defines various flags that can be used to describe the state of a window.
 *
 * @var WindowFlags::WINDOW_NO_FLAGS
 * No flags set. This indicates that the window has no special state.
 *
 * @var WindowFlags::WINDOW_PRERENDER_PASS
 * Flag indicating that the window is in prerender pass. This is used to specify that the window is currently in the prerendering phase.
 *
 * @var WindowFlags::WINDOW_RESIZED
 * Flag indicating that the window has been resized. This flag is set when the window's dimensions have changed.
 */
typedef enum WindowFlags {
    WINDOW_NO_FLAGS = 0,
    WINDOW_PRERENDER_PASS = 1 << 0,
    WINDOW_RESIZED = 1 << 1,
} WindowFlags;

/**
 * @brief Structure representing the SDL window and its associated OpenGL context.
 * 
 * This structure encapsulates the SDL window and its related properties, including
 * the OpenGL context, surfaces for rendering, and timing information.
 * 
 * @param window [in] The SDL_Window object representing the window.
 * @param context [in] The SDL_GLContext object representing the OpenGL context.
 * @param surface [in, out] The SDL_Surface object used for rendering.
 * @param width [in] The width of the window.
 * @param height [in] The height of the window.
 * @param lastFrameTime [in, out] The timestamp of the last rendered frame.
 * @param deltaTime [in, out] The time elapsed between the last two frames.
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
    u8 flags; /**< Flags for window properties. */
} Window;

struct WorldShaders;
struct Node;
struct Camera;
struct DepthMap;

/**
 * @brief Creates a new SDL window with an OpenGL context.
 * 
 * @param[in] title The title of the window.
 * @param[in] x The x-coordinate of the window's position on the screen.
 * @param[in] y The y-coordinate of the window's position on the screen.
 * @param[in] width The width of the window in pixels.
 * @param[in] height The height of the window in pixels.
 * @param[in] flags Flags to configure the window's behavior (e.g., fullscreen, resizable).
 * @param[out] window Pointer to the Window structure to be initialized.
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
 * @brief Updates the window by rendering the scene and swapping the buffers.
 * 
 * @param[in] window Pointer to the Window structure containing 
 *                   the window's state and timing information.
 * @param[in] scene Pointer to the Node structure representing 
 *                  the scene to be rendered.
 * @param[in] c Pointer to the Camera structure used for rendering 
 *              the scene.
 * @param[in] shaders Pointer to the WorldShaders structure used for rendering.
 * @param[in] depthMap Pointer to the DepthMap structure that handles 
 *                     depth information for rendering.
 * @param[in] msaa Pointer to the MSAA structure for multisample anti-aliasing.
 * @param[in] screenPlane Pointer to the Mesh structure representing the screen plane.
 * @return This function does not return a value.
 * 
 * This function calculates the time elapsed since the last update, 
 * updates the window's state accordingly, and calls the draw_screen function 
 * to render the scene's content. It also enables relative mouse mode 
 * for capturing mouse movements. Finally, it swaps the OpenGL buffers 
 * to display the rendered content on the window.
 */
void update_window(Window *window, struct Node *scene, struct Camera *c, struct WorldShaders *shaders, struct DepthMap *depthMap, struct MSAA *msaa, struct Mesh *screenPlane);

/**
 * @brief Refreshes the UI of the window.
 * 
 * This function updates the graphical user interface of the specified window.
 * It should be called whenever the window needs to be redrawn or updated.
 * 
 * @param[in] window Pointer to the Window structure that represents the window to be refreshed.
 */
void refresh_ui(Window *window);

/**
 * @brief Frees the resources associated with the window.
 * 
 * This function releases all the resources allocated for the given window,
 * ensuring that there are no memory leaks.
 * 
 * @param[in] window Pointer to the Window structure to be freed. This should
 *                   be a valid pointer to a previously created Window structure.
 */
void free_window(Window *window);

/** @} */ // end of WindowManagement group

#endif