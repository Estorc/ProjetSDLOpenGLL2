#include <raptiquax.h>
#include <math/math_util.h>
#include <io/model.h>
#include <render/framebuffer.h>
#include <storage/node.h>
#include <render/render.h>
#include <render/lighting.h>
#include <classes/classes.h>
#include <io/scene_loader.h>
#include <scripts/scripts.h>
#include <render/camera.h>
#include <storage/queue.h>

void change_scene() {

    Node **root = (Node **) queue_pop(Game.callQueue);
    char *path = (char *) queue_pop(Game.callQueue);
    Camera **camera = (Camera **) queue_pop(Game.callQueue);
    Script *scripts = (Script *) queue_pop(Game.callQueue);
    (*root)::free();
    (*root) = load_scene(path, camera, scripts);
    PRINT_INFO("Scene changed to %s\n", path);
    PRINT_INFO("Root: %p\n", *root);
    Game.window->flags |= WINDOW_PRERENDER_PASS; // Force a prerender pass to prevent weird rendering bugs
}

void prepare_change_scene(char *path) {
    queue_push(Game.callQueue, change_scene);
    queue_push(Game.callQueue, &Game.mainTree->root);
    queue_push(Game.callQueue, (void *) path);
    queue_push(Game.callQueue, &Game.camera);
    queue_push(Game.callQueue, Game.scripts);
}