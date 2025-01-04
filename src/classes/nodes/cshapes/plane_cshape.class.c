#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class PlaneCShape : public CShape {
    __containerType__ Node *
    public:

    void constructor(struct PlaneCollisionShape *planeCollisionShape) {
        this->object = planeCollisionShape;
        this->type = __type__;
        SUPER(initialize_node);
    }

    void cast(void ** data) {
        IGNORE(data);
    }

    void get_priority(int *priority) {
        *priority = 2;
    }

    void load() {
        PlaneCollisionShape *planeCollisionShape;
        planeCollisionShape = malloc(sizeof(PlaneCollisionShape));
        POINTER_CHECK(planeCollisionShape);
        METHOD_TYPE(this, __type__, constructor, planeCollisionShape);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }




    
}
