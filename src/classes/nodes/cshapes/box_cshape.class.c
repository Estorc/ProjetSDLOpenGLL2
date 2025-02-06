/**
 * @file
 * @brief This source file defines the BoxCShape class and its associated methods.
 *
 * This source file contains the implementation of the BoxCShape class, which is used
 * to represent and manipulate box-shaped collision shapes in the application. It provides
 * functionalities for creating, initializing, and managing box collision shapes, as well as
 * handling their interactions within the physics engine.
 *
 * The BoxCShape class is designed to be used in conjunction with other collision shapes
 * and physics entities to simulate realistic physical interactions in a 2D or 3D environment.
 * It is a crucial component of the physics simulation system, enabling accurate collision
 * detection and response for box-shaped objects.
 *
 * @author Loup Picault
 * @date 2023-10-06
 */

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
