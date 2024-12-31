#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class Texture extends Node {
    __containerType__ Node *

    void constructor(TextureMap *texture) {
        this->object = texture;
        this->type = __type__;
        SUPER(initialize_node);
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void load() {

    }

    void save() {

    }




    
}
