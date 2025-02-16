/**
 * @file
 * @brief This file defines the implementation of mesh shapes for the project.
 *
 * This source file contains the implementation of various mesh shapes used in the project.
 * It provides the necessary functions and data structures to create, manipulate, and render
 * mesh shapes within the SDL and OpenGL context.
 *
 * The main features include:
 * - Creation of different mesh shapes
 * - Functions to manipulate mesh vertices and indices
 * - Rendering functions to display the mesh shapes using OpenGL
 *
 * This file is a crucial part of the graphics rendering pipeline, ensuring that mesh shapes
 * are correctly created and displayed in the application.
 *
 * @author Loup Picault
 * @date 2023-10-25
 */

#include "raptiquax.h"
#include "classes/classes.h"
#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"

class MeshCShape : public CShape {
    __containerType__ Node *
    public:

    void constructor(struct MeshCollisionShape *meshCollisionShape) {
        this->object = meshCollisionShape;
        this->type = __type__;
        SUPER(initialize_node);
    }

    

    void get_priority(int *priority) {
        *priority = 4;
    }

    void load(FILE *file) {
        Model *model;
        model = malloc(sizeof(Model));
        if (file) {
            char path[100];
            fscanf(file,"(%100[^)])", path);
            load_model(path, &model->data);
        }
        MeshCollisionShape *meshCollisionShape;
        meshCollisionShape = malloc(sizeof(MeshCollisionShape));
        meshCollisionShape->facesVertex = model->data->objects[0].facesVertex;
        meshCollisionShape->numFaces = model->data->objects[0].length;
        POINTER_CHECK(meshCollisionShape);
        this->type = __type__;
        this::constructor(meshCollisionShape);
    }

    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }




    
}
