#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class BoxCShape : public CShape {
    __containerType__ Node *
    public:

    void constructor(struct BoxCollisionShape *boxCollisionShape) {
        this->object = boxCollisionShape;
        this->type = __type__;
        SUPER(initialize_node);
    }

    

    void get_priority(int *priority) {
        *priority = 0;
    }

    void load() {
        BoxCollisionShape *boxCollisionShape;
        boxCollisionShape = malloc(sizeof(BoxCollisionShape));
        POINTER_CHECK(boxCollisionShape);
        this->type = __type__;
        this::constructor(boxCollisionShape);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }








    static Model _model = {0};
    void get_model(Model **model) {
        if (!_model.data) {
            load_model("assets/models/collisionShapes/box.obj", &_model.data);
        }
        *model = &_model;
    }

    
}
