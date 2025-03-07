/**
 * @file
 * @brief This file defines the implementation of ray-casting shapes.
 *
 * This source file contains the implementation of various functions and data structures
 * used for ray-casting shapes in the context of a graphics application using SDL and OpenGL.
 * It is used for handling the creation, manipulation, and rendering of ray-casting shapes
 * within the application.
 *
 * @author Loup Picault
 * @date 2023-10-17
 */

#include "raptiquax.h"
#include "classes/classes.h"
#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class RayCShape : public CShape {
    __containerType__ Node *
    public:

    void constructor() {
        this->type = __type__;

        RayCollisionShape *rayCollisionShape;
        rayCollisionShape = malloc(sizeof(RayCollisionShape));
        POINTER_CHECK(rayCollisionShape);

        this->object = rayCollisionShape;
        SUPER(initialize_node);
    }

    

    void get_priority(int *priority) {
        *priority = 5;
    }

    void load() {
        this::constructor();
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }




    
}
