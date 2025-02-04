#include "raptiquax.h"
#include "utils/time.h"
#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"
#include "render/camera.h"
#include "render/depth_map.h"
#include "render/render.h"
#include "window.h"

s8 create_window(char *title, s32 x, s32 y, s32 width, s32 height, u32 flags, Window *window) {
    window->startTime = get_time_in_seconds();
    window->time = 0.0f;
    window->lastTime = 0.0f;
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        PRINT_ERROR("Failed to initialize the SDL2 library\n");
        return -1;
    }

    if (TTF_Init() < 0) {
        PRINT_ERROR("Failed to initialize the SDL2 TTF library\n");
        return -1;
    }

    if (Mix_Init(flags) < 0) {
        PRINT_ERROR("Could not initialize mixer\n");
        PRINT_ERROR("Mix_Init: %s\n", Mix_GetError());
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
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, HARDWARE_RENDERING);
    // Sync buffer swap with monitor refresh rate
    enum {
      ADAPTIVE_VSYNC = -1,
      IMMEDIATE = 0,
      VSYNC = 1
    };

    #ifdef DEBUG_GL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG); 
    #endif
    SDL_GL_SetSwapInterval(VSYNC);

    window->sdl_window = SDL_CreateWindow(title, x, y, width, height, flags);
    window->opengl_ctx = SDL_GL_CreateContext(window->sdl_window);


    if(!window->sdl_window) {
        PRINT_ERROR("Failed to create window: %s\n", SDL_GetError());
        return -1;
    }

    window->surface = SDL_GetWindowSurface(window->sdl_window);
    window->ui_surface = SDL_CreateRGBSurface(0,width,height,32,0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
	glGenTextures(1, &window->ui_texture);
	refresh_ui(window);

    if(!window->surface) {
        PRINT_ERROR("Failed to get the surface from the window\n");
        return -1;
    }
    glEnable(GL_DEPTH_TEST);



    #ifdef DEBUG_GL
    PRINT_INFO("OpenGL Version: %s\n", glGetString(GL_VERSION));
    PRINT_INFO("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    PRINT_INFO("OpenGL Renderer: %s\n", glGetString(GL_RENDERER));

    void APIENTRY openglDebugCallback(GLenum source, GLenum type, GLuint id,
                                    GLenum severity, GLsizei length,
                                    const GLchar* message, const void* userParam) {
        PRINT_ERROR("[OpenGL Debug] (%d): %s\n", id, message);

        if (severity == GL_DEBUG_SEVERITY_HIGH) {
            PRINT_ERROR("SEVERITY: HIGH. Terminating!\n");
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

void update_window(Window *window, Node *scene, Camera *c, WorldShaders *shaders, DepthMap *depthMap, MSAA *msaa, Mesh *screenPlane) {

    window->lastTime = window->time;
    window->time = get_time_in_seconds() - window->startTime;
    //SDL_SetRelativeMouseMode(SDL_TRUE);  


    draw_screen(window, scene, c, shaders, depthMap, msaa, screenPlane);
    SDL_GL_SwapWindow(window->sdl_window);
    

}

void refresh_resolution() {
    int window_width, window_height;
    get_resolution(&window_width, &window_height);
    SDL_FreeSurface(window.ui_surface);
    window.ui_surface = SDL_CreateRGBSurface(0,window_width,window_height,32,0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
    resize_msaa_framebuffer(&mainNodeTree.msaa);
    window.resized = true;
}


void free_window(Window *window) {
    SDL_FreeSurface(window->ui_surface);
    SDL_DestroyWindow(window->sdl_window);
    glDeleteTextures(1, &window->ui_texture);
    PRINT_INFO("Free window!\n");
}