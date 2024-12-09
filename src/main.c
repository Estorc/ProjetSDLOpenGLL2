#include "types.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_mixer.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <stdio.h>
#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "node.h"
#include "render/depth_map.h"
#include "render/render.h"
#include "render/lighting.h"
#include "window.h"
#include "io/input.h"
#include "io/stringio.h"
#include "io/osio.h"
#include "render/camera.h"
#include "io/shader.h"
#include "utils/skybox.h"
#include "io/scene_loader.h"
#include "physics/physics.h"
#include "physics/bodies.h"
#include "scripts/scripts.h"
#include "memory.h"
#include "buffer.h"

#include "classes/classes.h"

#include <SDL2/SDL_ttf.h>

int update(Window *window, Node *viewportNode, Camera *c, Input *input, WorldShaders *shaders, DepthMap *depthMap) {

    float delta = (window->lastTime) ? window->time - window->lastTime : 0.0;

    if (update_input(input)) return -1;
    if (input->released_keys & KEY_MENU) return -1;


    SDL_FillRect(window->ui_surface, NULL, 0x000000);
    buffers.collisionBuffer.index = 0;
    buffers.lightingBuffer.index = 0;
    u8 lightsCount[LIGHTS_COUNT] = {0};
    update_physics(((Viewport *) viewportNode->object)->scene, (vec3) {0.0, 0.0, 0.0}, (vec3) {0.0, 0.0, 0.0}, (vec3) {1.0, 1.0, 1.0}, delta, input, window, lightsCount, true);
    set_lightings(lightsCount);
    refresh_ui(window);
    update_window(window, viewportNode, c, shaders, depthMap);

    return 0;

}

MemoryCaches memoryCaches;
BufferCollection buffers;
//const struct ClassManager classManager;
BUILD_CLASS_METHODS_CORRESPONDANCE(classManager);

int main(int argc, char *argv[]) {

    if (update_cwd() == -1) return -1;
    init_memory_cache();
    static Script scripts[SCRIPTS_COUNT];
    static u8 scriptIndex;
    #include "scripts/loading_scripts.h"

    Window window;
    if (create_window("Physics Engine Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL, &window) == -1) return -1;
    
    Input input;
    init_input(&input);

    Camera *cam;

    WorldShaders defaultShaders = {
        .render = create_shader(DEFAULT_RENDER_SHADER),
        .depth = create_shader(DEFAULT_DEPTH_SHADER),
        .screen = create_shader(DEFAULT_SCREEN_SHADER)
    };

    DepthMap depthMap;
    create_depthmap(&depthMap, &defaultShaders);
    
    init_buffers();
    Node *viewportNode;

    Mix_OpenAudio(48000, AUDIO_S16SYS, 2, 2048);
    Mix_Music *music = Mix_LoadMUS("assets/audio/musics/test.mp3");
    Mix_PlayMusic(music, 1);

    #ifdef DEBUG
    if (argc >= 2 && !strcmp(argv[1], "editor")) viewportNode = load_scene("assets/scenes/editor.scene", &cam, scripts);
    else 
    #endif
    viewportNode = load_scene("assets/scenes/boot.scene", &cam, scripts);
    while (update(&window, viewportNode, cam, &input, &defaultShaders, &depthMap) >= 0);

    Mix_FreeMusic(music);
    free_buffers();
    free_memory_cache();
    free_node(viewportNode);
    printf("Free nodes!\n");
    free_window(&window);
    SDL_Quit();
    return 0;
}



