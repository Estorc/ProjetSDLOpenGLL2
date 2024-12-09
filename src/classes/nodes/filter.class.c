#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "node.h"

class Filter @promote extends Node {
    __containerType__ Node * 

    void constructor(struct Filter *filter) {
        this->object = filter;
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
