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
#include "utils/scene.h"

#include "classes/classes.h"

MemoryCaches memoryCaches;
BufferCollection buffers;
Queue callQueue = {NULL};
Tree mainNodeTree;
Input input;
Settings settings = {false, true, false, RES_RESPONSIVE, false};
Window window;

BUILD_CLASS_METHODS_CORRESPONDANCE(classManager);