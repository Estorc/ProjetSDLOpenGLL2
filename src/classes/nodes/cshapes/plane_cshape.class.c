/**
 * @file
 * @brief This file defines the implementation of a plane shape class.
 *
 * This source file contains the implementation details for the plane shape class,
 * which is a part of the node-based structure in the project. It provides the
 * necessary functions and data structures to create, manipulate, and render
 * plane shapes within the application.
 *
 * The plane shape class is designed to be used in conjunction with other shape
 * classes to build complex 3D models and scenes. It includes methods for
 * initializing the plane, setting its properties, and integrating it with the
 * rendering pipeline.
 *
 * @author Loup Picault
 * @date 2023-10-25
 */

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
