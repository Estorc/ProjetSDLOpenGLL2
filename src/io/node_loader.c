#include "../types.h"
#include "../math/math_util.h"
#include "model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
#include "../render/render.h"
#include "../render/lighting.h"
#include "../window.h"
#include "input.h"
#include "../render/camera.h"
#include "shader.h"
#include "../render/depth_map.h"
#include "scene_loader.h"
#include "../memory.h"
#include "../buffer.h"
#include "../classes/classes.h"

void malloc_node(Node *node, int nodeType, FILE *file, Camera **c, Script scripts[SCRIPTS_COUNT], Node *editor) {
    node::nodeType::constructor(NULL);
    node::load(file, c, scripts, editor);
}


void node_tree_to_file(FILE * file, Node *node, Node *editor) {
    node::save(file, editor);
}