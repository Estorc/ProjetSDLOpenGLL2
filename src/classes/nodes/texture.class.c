#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class Texture : public Node {
    __containerType__ Node *
    public:

    void constructor(TextureMap *texture) {
        this->object = texture;
        this->type = __type__;
        SUPER(initialize_node);
    }

    

    void load() {

    }

    void save() {

    }




    
}
