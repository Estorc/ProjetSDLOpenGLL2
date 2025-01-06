#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class RayCShape : public CShape {
    __containerType__ Node *
    public:

    void constructor(struct RayCollisionShape *rayCollisionShape) {
        this->object = rayCollisionShape;
        this->type = __type__;
        SUPER(initialize_node);
    }

    

    void get_priority(int *priority) {
        *priority = 5;
    }

    void load() {
        RayCollisionShape *rayCollisionShape;
        rayCollisionShape = malloc(sizeof(RayCollisionShape));
        POINTER_CHECK(rayCollisionShape);
        this->type = __type__;
        this::constructor(rayCollisionShape);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }




    
}
