#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class CShape : public Node {
    __containerType__ Node *
    public:

    void is_cshape(bool *cshape) {
        (*cshape) = true;
    }
    
}
