#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class Framebuffer : public Node {
    __containerType__ Node *
    public:

    void constructor(FrameBuffer *frameBuffer) {
        this->object = frameBuffer;
        this->type = __type__;
        SUPER(initialize_node);
    }

    

    void load() {
        FrameBuffer *msaa_framebuffer;
        msaa_framebuffer = malloc(sizeof(FrameBuffer));

        this::constructor(msaa_framebuffer);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }




    
}
