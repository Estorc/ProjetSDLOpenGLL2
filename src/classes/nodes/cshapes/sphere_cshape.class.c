#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "node.h"

class SphereCShape @promote extends CShape {
    __containerType__ Node *

    void constructor(struct SphereCollisionShape *sphereCollisionShape) {
        this->object = sphereCollisionShape;
        this->type = __type__;
        SUPER(initialize_node);
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void get_priority(int *priority) {
        *priority = 1;
    }

    void load() {
        SphereCollisionShape *sphereCollisionShape;
        sphereCollisionShape = malloc(sizeof(SphereCollisionShape));
        POINTER_CHECK(sphereCollisionShape);
        METHOD_TYPE(this, CLASS_TYPE_SPHERECSHAPE, constructor, sphereCollisionShape);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }




    
}
