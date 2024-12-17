#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class CShape @promote extends Node {
    __containerType__ Node *

    void is_cshape(bool *cshape) {
        (*cshape) = true;
    }
    
}
