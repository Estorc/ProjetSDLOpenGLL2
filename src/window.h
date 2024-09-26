#include "render/camera.h"
#include "io/shader.h"
#include "render/depth_map.h"

#define SCREEN_WIDTH   1280
#define SCREEN_HEIGHT  720
#define window_sleep(x) SDL_Delay(x)


#ifndef WINDOW_H
#define WINDOW_H

typedef struct Window {
    SDL_Window *sdl_window;
    SDL_Surface *surface;
    SDL_Surface *ui_surface;
    TextureMap ui_texture;
    SDL_GLContext opengl_ctx;
    float startTime;
    float time;
    float lastTime;
} Window;

struct WorldShaders;
s8 create_window(char *title, s32 x, s32 y, s32 width, s32 height, u32 flags, Window *window);
void update_window(Window *window, Node *viewportNode, Camera *c, struct WorldShaders *shaders, DepthMap *depthMap);
void refresh_ui(Window *window);
void free_window(Window *window);

#endif