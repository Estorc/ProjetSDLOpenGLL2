#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class BoxCShape @promote extends CShape {
    __containerType__ Node *

    void constructor(struct BoxCollisionShape *boxCollisionShape) {
        this->object = boxCollisionShape;
        this->type = __type__;
        SUPER(initialize_node);
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void get_priority(int *priority) {
        *priority = 0;
    }

    void load() {
        BoxCollisionShape *boxCollisionShape;
        boxCollisionShape = malloc(sizeof(BoxCollisionShape));
        POINTER_CHECK(boxCollisionShape);
        METHOD_TYPE(this, __type__, constructor, boxCollisionShape);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }




    
}
