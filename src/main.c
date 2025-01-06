#include "types.h"
#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"
#include "render/depth_map.h"
#include "render/render.h"
#include "render/lighting.h"
#include "window.h"
#include "io/input.h"
#include "io/osio.h"
#include "render/camera.h"
#include "io/shader.h"
#include "io/scene_loader.h"
#include "io/node_loader.h"
#include "physics/physics.h"
#include "physics/bodies.h"
#include "scripts/scripts.h"
#include "gui/frame.h"
#include "settings.h"
#include "memory.h"
#include "buffer.h"
#include "storage/queue.h"
#include "utils/scene.h"

#include "classes/classes.h"

float accumulator = 0.0f;
const float fixedTimeStep = 0.0167f;
float fps = 0.0f;

int update(Window *window, WorldShaders *shaders, DepthMap *depthMap, MSAA *msaa, Mesh *screenPlane) {
    float delta = (window->lastTime) ? window->time - window->lastTime : 0.0;
    const float maxDelta = 0.1f;
    delta = (delta > maxDelta) ? maxDelta : delta;
    accumulator += delta;

    if (accumulator >= fixedTimeStep) SDL_FillRect(window->ui_surface, NULL, 0x000000);

    while (!queue_is_empty(&callQueue)) {
        void(*call)() = queue_pop(&callQueue);
        if (call) call();
        else return -1;
    }

 
    char delta_str[50];
    char fps_str[50];
    if (settings.show_fps) {
        sprintf(delta_str, "DELTA: %.4f", delta);
        if (delta) {
            fps = (fps+(1.0/delta))/2.0;
            sprintf(fps_str, "FPS: %.4f", fps);
        }

        TTF_Font *font = TTF_OpenFont("assets/fonts/determination-mono.ttf", 48);
        SDL_Color textColor = {255, 255, 255, 255};
        draw_text(window->ui_surface, 8, 0, delta_str, font, textColor, "lt", -1);
        draw_text(window->ui_surface, 8, 32, fps_str, font, textColor, "lt", -1);
        TTF_CloseFont(font);
    }

    u8 lightsCount[LIGHTS_COUNT];
    while (accumulator >= fixedTimeStep) {
        s8 input_result = update_input(&input);
        if (input_result) {
            if (input_result == -1) return -1;
            if (input_result == 1) {
                refresh_resolution();
            }
        }

        buffers.collisionBuffer.index = 0;
        buffers.lightingBuffer.index = 0;
        memset(lightsCount, 0, sizeof(lightsCount));
        update_physics(mainNodeTree.root, (vec3) {0.0, 0.0, 0.0}, (vec3) {0.0, 0.0, 0.0}, (vec3) {1.0, 1.0, 1.0}, fixedTimeStep, &input, window, lightsCount, true);
        window->resized = false;
        accumulator -= fixedTimeStep;
    }
    set_lightings(lightsCount);
    refresh_ui(window);
    update_window(window, mainNodeTree.root, mainNodeTree.camera, shaders, depthMap, msaa, screenPlane);

    return 0;
}


MemoryCaches memoryCaches;
BufferCollection buffers;
Queue callQueue = {NULL};
Tree mainNodeTree;
Input input;
Settings settings = {false, true, false, RES_RESPONSIVE, false};
Window window;

BUILD_CLASS_METHODS_CORRESPONDANCE(classManager);


int main(int argc, char *argv[]) {

    if (update_cwd() == -1) return -1;
    init_memory_cache();
    #include "scripts/loading_scripts.h"

    if (create_window("Physics Engine Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL, &window) == -1) return -1;
    init_input(&input);


    // TODO: Transform this in singletons

    WorldShaders defaultShaders = {
        .render = create_shader(DEFAULT_RENDER_SHADER),
        .depth = create_shader(DEFAULT_DEPTH_SHADER),
        .screen = create_shader(DEFAULT_SCREEN_SHADER),
        .skybox = create_shader(DEFAULT_SKYBOX_SHADER),
        .gui = create_shader(DEFAULT_GUI_SHADER)
    };

    DepthMap depthMap;
    create_depthmap(&depthMap, &defaultShaders);

    Mesh screenPlane;
    create_screen_plane(&screenPlane);

    create_msaa_framebuffer(&mainNodeTree.msaa);


    
    init_buffers();

    Mix_OpenAudio(48000, AUDIO_S16SYS, 2, 2048);
    Mix_Music *music = Mix_LoadMUS("assets/audio/musics/test.mp3");
    Mix_PlayMusic(music, 1);

    #ifdef DEBUG
    if (argc >= 2 && !strcmp(argv[1], "editor")) mainNodeTree.root = load_scene("assets/scenes/editor.scene", &mainNodeTree.camera, mainNodeTree.scripts);
    else 
    #endif
    mainNodeTree.root = load_scene("assets/scenes/boot.scene", &mainNodeTree.camera, mainNodeTree.scripts);

    while (update(&window, &defaultShaders, &depthMap, &mainNodeTree.msaa, &screenPlane) >= 0);

    Mix_FreeMusic(music);

    if (!queue_is_empty(&callQueue)) {
        queue_free(&callQueue);
        printf("Free call queue!\n");
    }

    free_msaa_framebuffer(&mainNodeTree.msaa);

    free_buffers();
    free_memory_cache();
    printf("Free nodes!\n");
    (mainNodeTree.root)::free();

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    glDeleteTextures(1, &depthMap.texture);
    glDeleteFramebuffers(1, &depthMap.frameBuffer);
    glDeleteBuffers(1, &depthMap.ubo);
    printf("Free depth map!\n");

    free_window(&window);
    SDL_Quit();
    return 0;
}



