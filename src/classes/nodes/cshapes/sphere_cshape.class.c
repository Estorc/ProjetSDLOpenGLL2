#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class SphereCShape : public CShape {
    __containerType__ Node *
    public:

    void constructor(struct SphereCollisionShape *sphereCollisionShape) {
        this->object = sphereCollisionShape;
        this->type = __type__;
        SUPER(initialize_node);
    }

    

    void get_priority(int *priority) {
        *priority = 1;
    }

    void load() {
        SphereCollisionShape *sphereCollisionShape;
        sphereCollisionShape = malloc(sizeof(SphereCollisionShape));
        POINTER_CHECK(sphereCollisionShape);
        this->type = __type__;
        this::constructor(sphereCollisionShape);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }

    static Model _model = {0};
    void get_model(Model **model) {
        if (!_model.data) {
            load_model("assets/models/collisionShapes/sphere.obj", &_model.data);
        }
        *model = &_model;
    }


    
}
