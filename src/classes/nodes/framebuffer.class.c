#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class Framebuffer @promote extends Node {
    __containerType__ Node *

    void constructor(FrameBuffer *frameBuffer) {
        this->object = frameBuffer;
        this->type = __type__;
        SUPER(initialize_node);
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void load() {
        FrameBuffer *msaa_framebuffer;
        msaa_framebuffer = malloc(sizeof(FrameBuffer));

        METHOD(this, constructor, msaa_framebuffer);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }




    
}
