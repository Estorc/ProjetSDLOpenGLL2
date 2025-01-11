#line 1 "src/utils/scene.c"
#include "../types.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
#include "../render/render.h"
#include "../render/lighting.h"
#include "../classes/classes.h"
#include "../io/scene_loader.h"
#include "../scripts/scripts.h"
#include "../render/camera.h"
#include "../storage/queue.h"

void change_scene() {

    Node **root = (Node **) queue_pop(&callQueue);
    char *path = (char *) queue_pop(&callQueue);
    Camera **camera = (Camera **) queue_pop(&callQueue);
    Script *scripts = (Script *) queue_pop(&callQueue);
    call_method_0(METHOD(free,(*root)));
    (*root) = load_scene(path, camera, scripts);
    printf("Scene changed to %s\n", path);
    printf("Root: %p\n", *root);
}

void prepare_change_scene(char *path) {
    queue_push(&callQueue, change_scene);
    queue_push(&callQueue, &mainNodeTree.root);
    queue_push(&callQueue, (void *) path);
    queue_push(&callQueue, &mainNodeTree.camera);
    queue_push(&callQueue, mainNodeTree.scripts);
}