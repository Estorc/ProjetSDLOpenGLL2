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
    (*root)::free();
    (*root) = load_scene(path, camera, scripts);
    printf("Scene changed to %s\n", path);
    printf("Root: %p\n", *root);
}