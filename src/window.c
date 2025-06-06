#include <raptiquax.h>
#include <utils/time.h>
#include <math/math_util.h>
#include <io/model.h>
#include <render/framebuffer.h>
#include <storage/node.h>
#include <render/camera.h>
#include <render/depth_map.h>
#include <render/render.h>
#include <window.h>
#include <settings.h>

#ifdef DEBUG
//#define DEBUG_GL
#endif

#ifdef DEBUG_GL

#ifdef _WIN32
    #define APIENTRY __stdcall
#endif


void APIENTRY openglDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    
    #ifndef ENABLE_OPENGL_NOTIFICATIONS
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
        return;
    }
    #endif
    
    const char* sourceStr = NULL;
    const char* typeStr = NULL;
    const char* severityStr = NULL;

    // Convert the enums to strings for more detailed information
    switch (source) {
        case GL_DEBUG_SOURCE_API: sourceStr = "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceStr = "Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceStr = "Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY: sourceStr = "Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION: sourceStr = "Application"; break;
        case GL_DEBUG_SOURCE_OTHER: sourceStr = "Other"; break;
        default: sourceStr = "Unknown"; break;
    }

    switch (type) {
        case GL_DEBUG_TYPE_ERROR: typeStr = "Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "Deprecated Behavior"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: typeStr = "Undefined Behavior"; break;
        case GL_DEBUG_TYPE_PORTABILITY: typeStr = "Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE: typeStr = "Performance"; break;
        case GL_DEBUG_TYPE_OTHER: typeStr = "Other"; break;
        default: typeStr = "Unknown"; break;
    }

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH: severityStr = "High"; break;
        case GL_DEBUG_SEVERITY_MEDIUM: severityStr = "Medium"; break;
        case GL_DEBUG_SEVERITY_LOW: severityStr = "Low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: severityStr = "Notification"; break;
        default: severityStr = "Unknown"; break;
    }

    // Output the debug message with additional information like file and line number
    if (type != GL_DEBUG_TYPE_ERROR) {
        PRINT_WARNING("[OpenGL Debug] (%d) Source: %s, Type: %s, Severity: %s\nMessage: %s\n", id, sourceStr, typeStr, severityStr, message);
    } else {
        PRINT_ERROR("[OpenGL Debug] (%d) Source: %s, Type: %s, Severity: %s\nMessage: %s\n", id, sourceStr, typeStr, severityStr, message);
    }

    if (severity == GL_DEBUG_SEVERITY_HIGH) {
    PRINT_ERROR("SEVERITY: HIGH. Terminating!\n");
    abort(); // Stop execution if severity is high
    }
}

#endif

s8 create_window(char *title, s32 x, s32 y, s32 width, s32 height, u32 flags, Window *window) {
    window->flags = WINDOW_PRERENDER_PASS;
    window->startTime = get_time_in_seconds();
    window->time = 0.0f;
    window->lastTime = 0.0f;
    glm_vec4_zero(window->fadeColor);
    window->fadeColor[3] = 1.0f;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        PRINT_ERROR("Failed to initialize the SDL2 library\n");
        return -1;
    }

    if (init_video_player() == 0) {
        fprintf(stderr, "Failed to open audio device: %s\n", SDL_GetError());
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

    if (Mix_AllocateChannels(256) < 0) {
        PRINT_ERROR("Could not allocate channels\n");
        PRINT_ERROR("Mix_AllocateChannels: %s\n", Mix_GetError());
        return -1;
    }

    // Disable deprecated functions
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
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
    if (Game.settings->vsync)
        SDL_GL_SetSwapInterval(VSYNC);
    else
        SDL_GL_SetSwapInterval(IMMEDIATE);

    window->sdl_window = SDL_CreateWindow(title, x, y, width, height, flags);
    window->opengl_ctx = SDL_GL_CreateContext(window->sdl_window);


    #ifdef _WIN32

    if (glewInit() != GLEW_OK) {
        PRINT_ERROR("Failed to initialize GLEW\n");
        return -1;
    }

    #endif


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

    PRINT_INFO("OpenGL Version: %s\n", glGetString(GL_VERSION));
    PRINT_INFO("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    PRINT_INFO("OpenGL Renderer: %s\n", glGetString(GL_RENDERER));

    #ifdef DEBUG_GL

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

void update_window(Window *window, Node *scene, Camera *c, WorldShaders *shaders, DepthMap *depthMap, Mesh *screenPlane) {



    draw_screen(window, scene, c, shaders, depthMap, screenPlane);

    window->flags &= ~WINDOW_RESIZED;
    if (window->flags & WINDOW_PRERENDER_PASS) {
        window->flags &= ~WINDOW_PRERENDER_PASS;
        return;
    }
    window->lastTime = window->time;
    SDL_GL_SwapWindow(window->sdl_window);
    window->time = get_time_in_seconds() - window->startTime;

}

void refresh_resolution() {
    int window_width, window_height;
    get_resolution(&window_width, &window_height);
    SDL_FreeSurface(Game.window->ui_surface);
    Game.window->ui_surface = SDL_CreateRGBSurface(0,window_width,window_height,32,0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
    resize_cfbo(Game.uiFBO);
    resize_dfbo(Game.deferredBuffer);
    resize_intermediate_fbo();
    set_shaders_screen_size(window_width, window_height);
    Game.window->flags |= WINDOW_PRERENDER_PASS;
    Game.window->flags |= WINDOW_RESIZED;
}


void free_window(Window *window) {
    SDL_FreeSurface(window->ui_surface);
    SDL_DestroyWindow(window->sdl_window);
    glDeleteTextures(1, &window->ui_texture);
    free_video_player();
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    PRINT_INFO("Free window!\n");
}