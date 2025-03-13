#include "raptiquax.h"
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
#include "storage/hash.h"
#include "utils/scene.h"

#include "classes/classes.h"

#define BOOT_SCENE "assets/scenes/claude_chappe/boot.scene"

float accumulator = 0.0f;
const float fixedTimeStep = 0.0167f;
float fps = 0.0f;

int update(Window *window, WorldShaders *shaders, DepthMap *depthMap, MSAA *msaa, Mesh *screenPlane) {
    float delta = (window->lastTime) ? window->time - window->lastTime : 0.0;
    const float maxDelta = 0.1f;
    delta = (delta > maxDelta) ? maxDelta : delta;
    accumulator += delta;

    if (accumulator >= fixedTimeStep) SDL_FillRect(window->ui_surface, NULL, 0x000000);

    while (!queue_is_empty(Game.callQueue)) {
        void(*call)() = queue_pop(Game.callQueue);
        if (call) call();
        else return -1;
    }

 
    char delta_str[50];
    char fps_str[50];
    if (Game.settings->show_fps) {
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
        s8 input_result = update_input(Game.input);
        if (input_result) {
            if (input_result == -1) return -1;
            if (input_result == 1) {
                refresh_resolution();
            }
        }

        Game.buffers->collisionBuffer.index = 0;
        Game.buffers->lightingBuffer.index = 0;
        memset(lightsCount, 0, sizeof(lightsCount));
        update_physics(Game.mainTree->root, (vec3) {0.0, 0.0, 0.0}, (vec3) {0.0, 0.0, 0.0}, (vec3) {1.0, 1.0, 1.0}, fixedTimeStep, Game.input, window, lightsCount, true);
        window->resized = false;
        accumulator -= fixedTimeStep;
    }
    set_lightings(lightsCount);
    refresh_ui(window);
    update_window(window, Game.mainTree->root, Game.camera, shaders, depthMap, msaa, screenPlane);

    return 0;
}


int main(int argc, char *argv[]) {

    if (update_cwd() == -1) return -1;
    init_memory_cache();

    if (create_window("Physics Engine Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE, Game.window) == -1) return -1;
    init_input(Game.input);

    Game.storage = table_create(16);

    
    // TODO: Transform this in singletons

    WorldShaders defaultShaders = {
        .render = create_shader(DEFAULT_RENDER_SHADER),
        .depth = create_shader(DEFAULT_DEPTH_SHADER),
        .screen = create_shader(DEFAULT_SCREEN_SHADER),
        .skybox = create_shader(DEFAULT_SKYBOX_SHADER),
        .gui = create_shader(DEFAULT_GUI_SHADER)
    };

    create_depthmap(Game.depthMap, &defaultShaders);

    Mesh screenPlane;
    create_screen_plane(&screenPlane);

    create_msaa_framebuffer(Game.msaa);


    
    init_buffers();

    Mix_OpenAudio(48000, AUDIO_S16SYS, 2, 2048);

    #ifdef DEBUG
    if (argc >= 2) {
        char scene[256] = "assets/scenes/";
        strcat(scene, argv[1]);
        strcat(scene, ".scene");
        Game.mainTree->root = load_scene(scene, &Game.camera, Game.scripts);
    }
    else 
    #endif
    Game.mainTree->root = load_scene(BOOT_SCENE, &Game.camera, Game.scripts);

    while (update(Game.window, &defaultShaders, Game.depthMap, Game.msaa, &screenPlane) >= 0);

    if (!queue_is_empty(Game.callQueue)) {
        queue_free(Game.callQueue);
        PRINT_INFO("Free call queue!\n");
    }


    PRINT_INFO("Free scripts!\n");
    free(Game.scripts);

    free_msaa_framebuffer(Game.msaa);

    free_buffers();
    free_memory_cache();
    PRINT_INFO("Free nodes!\n");
    (Game.mainTree->root)::free();

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    glDeleteTextures(1, &Game.depthMap->texture);
    for (int i = 0; i < NUM_DIRECTIONAL_LIGHTS + NUM_POINT_LIGHTS * 6 + NUM_SPOT_LIGHTS; i++) {
        glDeleteFramebuffers(1, &Game.depthMap->frameBuffer[i]);
    }
    glDeleteBuffers(1, &Game.depthMap->tbo);
    glDeleteTextures(1, &Game.depthMap->matrixTexture);
    PRINT_INFO("Free depth map!\n");

    table_free(Game.storage);
    PRINT_INFO("Free storage!\n");
    
    free_window(Game.window);
    SDL_Quit();
    return 0;
}



