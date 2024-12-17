#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"
#include "io/scene_loader.h"
#include "render/lighting.h"
#include "buffer.h"

class Body @promote extends Node {
    __containerType__ Node *

    void is_body(bool *body) {
        (*body) = true;
    }

    void free() {
        u8 *length;
        Node ***shapes;
        GET_FROM_BODY_NODE(this, length, length);
        GET_FROM_BODY_NODE(this, collisionsShapes, shapes);
        for (int j = 0; j < *length; j++) {
            free((*shapes)[j]);
        }
        free(*shapes);
        SUPER(free);
    }

    void apply_impulse() {
        // Do nothing
    }
}