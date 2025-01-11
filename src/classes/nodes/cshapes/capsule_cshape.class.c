#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class CapsuleCShape : public CShape {
    __containerType__ Node *
    public:

    void constructor(struct CapsuleCollisionShape *capsuleCollisionShape) {
        this->object = capsuleCollisionShape;
        this->type = __type__;
        SUPER(initialize_node);
    }

    

    void get_priority(int *priority) {
        *priority = 3;
    }

    void load() {
        CapsuleCollisionShape *capsuleCollisionShape;
        capsuleCollisionShape = malloc(sizeof(CapsuleCollisionShape));
        POINTER_CHECK(capsuleCollisionShape);
        this->type = __type__;
        this::constructor(capsuleCollisionShape);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }




    
}
