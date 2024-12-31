#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"
#include "io/scene_loader.h"
#include "render/lighting.h"
#include "buffer.h"

class Body extends PhysicalNode {
    __containerType__ Node *

    void is_body(bool *body) {
        (*body) = true;
    }

    void apply_impulse() {
        // Do nothing
    }
}