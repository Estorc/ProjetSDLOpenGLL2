/**
 * @file
 * @brief This file defines the implementation of the capsule shape class.
 *
 * This file contains the implementation details for the capsule shape class,
 * which is used to represent and manipulate capsule-shaped objects within the
 * application. It includes functions for creating, rendering, and managing
 * the properties of capsule shapes.
 * 
 * Detailed description:
 * The capsule shape class provides a way to create and handle capsule-shaped
 * objects in the application. It includes methods for initializing the shape,
 * setting its dimensions, and rendering it using OpenGL. This class is part
 * of the nodes module and interacts with other shape classes to provide a
 * comprehensive set of geometric shapes for the application.
 * 
 * @author Loup Picault
 * @date 2023-10-31
 */

#include "raptiquax.h"
#include "classes/classes.h"
#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class CapsuleCShape : public CShape {
    __containerType__ Node *
    public:

    void constructor() {
        this->type = __type__;

        CapsuleCollisionShape *capsuleCollisionShape;
        capsuleCollisionShape = malloc(sizeof(CapsuleCollisionShape));
        POINTER_CHECK(capsuleCollisionShape);

        this->object = capsuleCollisionShape;
        SUPER(initialize_node);
    }

    

    void get_priority(int *priority) {
        *priority = 3;
    }

    void load() {
        this::constructor();
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
