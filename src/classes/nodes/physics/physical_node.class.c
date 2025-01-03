#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"
#include "io/scene_loader.h"
#include "render/lighting.h"
#include "buffer.h"

class PhysicalNode extends Node {
    __containerType__ Node *

    void free() {
        u8 length;
        Node **shapes;
        METHOD(this, get_collisions_shapes, &shapes, &length);
        for (int j = 0; j < length; j++) {
            free(shapes[j]);
        }
        free(shapes);
        SUPER(free);
    }
}