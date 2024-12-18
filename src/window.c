#include "types.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_mixer.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include "utils/time.h"
#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"
#include "render/color.h"
#include "render/camera.h"
#include "render/depth_map.h"
#include "render/render.h"
#include "window.h"

/**
 * Creates a new SDL window with an OpenGL context.
 * 
 * @param title {char*} The title of the window.
 * @param x {s32} The x-coordinate of the window's position on the screen.
 * @param y {s32} The y-coordinate of the window's position on the screen.
 * @param width {s32} The width of the window in pixels.
 * @param height {s32} The height of the window in pixels.
 * @param flags {u8} Flags to configure the window's behavior (e.g., fullscreen, resizable).
 * @param window {Window*} Pointer to the Window structure to be initialized.
 * @return {s8} Returns 0 on success, or -1 on failure.
 * 
 * This function initializes the SDL2 library and creates a window with an OpenGL 
 * context. It sets various OpenGL attributes, including enabling double-buffering 
 * and depth testing. The window's start time and initial timing values are also 
 * initialized. If window creation or surface retrieval fails, an error message 
 * is printed and -1 is returned.
 */

s8 create_window(char *title, s32 x, s32 y, s32 width, s32 height, u32 flags, Window *window) {
    window->startTime = get_time_in_seconds();
    window->time = 0.0f;
    window->lastTime = 0.0f;
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("Failed to initialize the SDL2 library\n");
        return -1;
    }

    if (TTF_Init() < 0) {
        printf("Failed to initialize the SDL2 TTF library\n");
        return -1;
    }

    if (Mix_Init(flags) < 0) {
        printf("Could not initialize mixer\n");
        printf("Mix_Init: %s\n", Mix_GetError());
        return -1;
    }

    // Disable deprecated functions
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // Use double-buffering
    enum {
      SINGLE_BUFFER = 0,
      DOUBLE_BUFFER = 1,
    };
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, DOUBLE_BUFFER);
    enum {
      SOFTWARE_RENDERING = 0,
      HARDWARE_RENDERING = 1,
    };
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, SOFTWARE_RENDERING);
    // Sync buffer swap with monitor refresh rate
    enum {
      ADAPTIVE_VSYNC = -1,
      IMMEDIATE = 0,
      VSYNC = 1
    };

    #ifdef DEBUG
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG); 
    #endif
    SDL_GL_SetSwapInterval(ADAPTIVE_VSYNC);

    window->sdl_window = SDL_CreateWindow(title, x, y, width, height, flags);
    window->opengl_ctx = SDL_GL_CreateContext(window->sdl_window);


    if(!window->sdl_window) {
        printf("Failed to create window\n");
        return -1;
    }

    window->surface = SDL_GetWindowSurface(window->sdl_window);
    window->ui_surface = SDL_CreateRGBSurface(0,width,height,32,0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
	glGenTextures(1, &window->ui_texture);
	refresh_ui(window);

    if(!window->surface) {
        printf("Failed to get the surface from the window\n");
        return -1;
    }
    glEnable(GL_DEPTH_TEST);



    #ifdef DEBUG
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("OpenGL Renderer: %s\n", glGetString(GL_RENDERER));

    void APIENTRY openglDebugCallback(GLenum source, GLenum type, GLuint id,
                                    GLenum severity, GLsizei length,
                                    const GLchar* message, const void* userParam) {
        fprintf(stderr, "[OpenGL Debug] (%d): %s\n", id, message);

        if (severity == GL_DEBUG_SEVERITY_HIGH) {
            fprintf(stderr, "SEVERITY: HIGH. Terminating!\n");
            abort(); // Stop execution if severity is high
        }
    }

        // Enable OpenGL Debugging
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(openglDebugCallback, NULL);
    #endif
    return 0;
}

void refresh_ui(Window *window) {
    glBindTexture(GL_TEXTURE_2D, window->ui_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, window->ui_surface->w, window->ui_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, window->ui_surface->pixels);
    
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 * Updates the window by rendering the scene and swapping the buffers.
 * 
 * @param window {Window*} Pointer to the Window structure containing 
 *                         the window's state and timing information.
 * @param viewportNode {Node*} Pointer to the Node structure representing 
 *                             the viewport to be rendered.
 * @param c {Camera*} Pointer to the Camera structure used for rendering 
 *                    the scene.
 * @param shaders {Shader[]} Array of Shader structures used for rendering.
 * @param depthMap {DepthMap*} Pointer to the DepthMap structure that handles 
 *                              depth information for rendering.
 * @return {void} This function does not return a value.
 * 
 * This function calculates the time elapsed since the last update, 
 * updates the window's state accordingly, and calls the draw_screen function 
 * to render the viewport's content. It also enables relative mouse mode 
 * for capturing mouse movements. Finally, it swaps the OpenGL buffers 
 * to display the rendered content on the window.
 */

void update_window(Window *window, Node *scene, Camera *c, WorldShaders *shaders, DepthMap *depthMap, MSAA *msaa, Mesh *screenPlane) {

    window->lastTime = window->time;
    window->time = get_time_in_seconds() - window->startTime;
    //SDL_SetRelativeMouseMode(SDL_TRUE);  


    draw_screen(window, scene, c, shaders, depthMap, msaa, screenPlane);
    SDL_GL_SwapWindow(window->sdl_window);
    

}

void refresh_resolution() {
    resize_msaa_framebuffer(&mainNodeTree.msaa);
    window.resized = true;
}


void free_window(Window *window) {
    SDL_FreeSurface(window->ui_surface);
    SDL_DestroyWindow(window->sdl_window);
    glDeleteTextures(1, &window->ui_texture);
    printf("Free window!\n");
}