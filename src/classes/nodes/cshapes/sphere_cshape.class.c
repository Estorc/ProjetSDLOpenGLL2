/**
 * @file
 * @brief This file contains the implementation of the SphereCShape class.
 *
 * This source file provides the functionality for creating and manipulating
 * spherical shapes within the application. It includes methods for initializing,
 * rendering, and updating the properties of a sphere. The SphereCShape class
 * is designed to be used in conjunction with the OpenGL rendering pipeline
 * to display 3D spherical objects.
 *
 * The code in this file is intended to facilitate the creation of complex
 * 3D scenes by providing a reusable and efficient way to handle spherical
 * geometries.
 *
 * @author Loup Picault
 * @date 2023-10-20
 */

#include "raptiquax.h"
#include "classes/classes.h"
#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class SphereCShape : public CShape {
    __containerType__ Node *
    public:

    void constructor() {
        this->type = __type__;

        SphereCollisionShape *sphereCollisionShape;
        sphereCollisionShape = malloc(sizeof(SphereCollisionShape));
        POINTER_CHECK(sphereCollisionShape);

        this->object = sphereCollisionShape;
        SUPER(initialize_node);
    }

    

    void get_priority(int *priority) {
        *priority = 1;
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
