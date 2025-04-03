#include "raptiquax.h"
#include "math/math_util.h"
#include "io/model.h"
#include "io/input.h"
#include "render/framebuffer.h"
#include "storage/node.h"
#include "render/depth_map.h"
#include "render/render.h"
#include "render/lighting.h"
#include "window.h"
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

MemoryCaches memoryCaches;
BufferCollection buffers;
Queue callQueue = {NULL};
Tree mainTree = {0};
Input input;
Settings settings = {
    .show_fps = false, 
    .cast_shadows = true, 
    .window_fullscreen = false, 
    .resolution = RES_RESPONSIVE, 
    .show_collision_boxes = false, 
    .master_volume = 1, 
    .music_volume = 1, 
    .sfx_volume = 1, 
    .fov = PI/4.0f,
    .shadow_resolution = 256,
    .shadow_quality = 0,
    .texture_quality = 1,
    .antialiasing = true,
    .vsync = true
};
Window window;
MSAA msaa;


BUILD_CLASS_METHODS_CORRESPONDANCE(classManager);

struct RaptiquaX_t Game = {
    &memoryCaches, 
    &buffers, 
    &callQueue, 
    &mainTree, 
    &input, 
    &settings, 
    &window, 
    &classManager,
    NULL,
    &msaa,
    NULL,
    NULL
};