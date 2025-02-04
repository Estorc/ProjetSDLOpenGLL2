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

    

    void get_priority(int *priority) {
        *priority = 2;
    }

    void load() {
        PlaneCollisionShape *planeCollisionShape;
        planeCollisionShape = malloc(sizeof(PlaneCollisionShape));
        POINTER_CHECK(planeCollisionShape);
        this->type = __type__;
        this::constructor(planeCollisionShape);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }

    static Model _model = {0};
    void get_model(Model **model) {
        if (!_model.data) {
            load_model("assets/models/collisionShapes/plane.obj", &_model.data);
        }
        *model = &_model;
    }


    
}
