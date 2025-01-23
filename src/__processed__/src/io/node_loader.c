#line 1 "src/io/node_loader.c"
#include "../raptiquax.h"
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
    call_method_0(METHOD_TYPE(nodeType,constructor,node,NULL));
    call_method_0(METHOD(load,node,file, c, scripts, editor));
}


void node_tree_to_file(FILE * file, Node *node, Node *editor) {
    call_method_0(METHOD(save,node,file, editor));
}