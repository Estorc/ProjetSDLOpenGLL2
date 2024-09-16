#include "types.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <stdio.h>
#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "node.h"
#include "window.h"
#include "io/input.h"
#include "render/camera.h"
#include "io/shader.h"
#include "utils/skybox.h"
#include "render/depth_map.h"
#include "io/scene_loader.h"
#include "physics/physics.h"
#include "physics/bodies.h"
#include "scripts/scripts.h"
#include "memory.h"

#include <SDL2/SDL_ttf.h>

int FPS[2] = {0,0};
float fps = 0;
int update(Window *window, Node *viewportNode, Camera *c, Input *input, Shader shaders[], DepthMap *depthMap, CollisionBuffer *collisionBuffer) {

    float delta = (window->lastTime) ? window->time - window->lastTime : 0.0;

    if (update_input(input)) return -1;
    if (input->released_keys & KEY_MENU) return -1;


    /*FPS[(((int) window->time)%2)]++;
    if (FPS[!(((int) window->time)%2)]) {
        if (fps < 2.0) fps = FPS[!(((int) window->time)%2)];
        else fps = (fps+FPS[!(((int) window->time)%2)])/2.0;
        fprintf(stdout, "FPS: %f\n", fps);
        FPS[!(((int) window->time)%2)] = 0;
    }*/

    SDL_FillRect(window->ui_surface, NULL, 0x000000);
    collisionBuffer->length = 0;
    update_physics(((Viewport *) viewportNode->object)->scene, (vec3) {0.0, 0.0, 0.0}, (vec3) {0.0, 0.0, 0.0}, (vec3) {1.0, 1.0, 1.0}, delta, collisionBuffer, input, window, true);
    refresh_ui(window);
    update_window(window, viewportNode, c, shaders, depthMap);

    return 0;

}

MemoryCaches memoryCaches;

#define NUM_SHADERS 5
int main(int argc, char *argv[]) {

    init_memory_cache();
    static Script scripts[SCRIPTS_COUNT];
    static u8 scriptIndex;
    #include "scripts/loading_scripts.h"

    Window window;
    if (create_window("Physics Engine Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_MOUSE_GRABBED, &window) == -1) return -1;

    Input input;
    init_input(&input);

    Camera *cam;

    Shader shaders[NUM_SHADERS];
    create_shaders(shaders);

    DepthMap depthMap;
    create_depthmap(&depthMap);
    

    CollisionBuffer collisionBuffer;
    Node *viewportNode;
    if (argc >= 2 && !strcmp(argv[1], "editor")) viewportNode = load_scene("scenes/editor.scene", &cam, &collisionBuffer, scripts);
    else viewportNode = load_scene("scenes/boot.scene", &cam, &collisionBuffer, scripts);
    while (update(&window, viewportNode, cam, &input, shaders, &depthMap, &collisionBuffer) >= 0);

    free(collisionBuffer.collisionsShapes);
    free_memory_cache();
    free_node(viewportNode);
    printf("Free nodes!\n");
    free_window(&window);
    SDL_Quit();
    return 0;
}



