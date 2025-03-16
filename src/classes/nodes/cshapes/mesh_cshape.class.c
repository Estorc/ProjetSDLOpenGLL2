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

/**
 * @ingroup Classes Classes
 * @{
 */
class MeshCShape : public CShape {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the mesh_cshape class.
     *
     * This function initializes a mesh_cshape object using the provided file path.
     *
     * @param path The file path to initialize the mesh_cshape object.
     */
    void constructor(const char *path) {
        this->type = __type__;

        Model *model;
        model = malloc(sizeof(Model));
        POINTER_CHECK(model);
        load_model(path, &model->data);

        MeshCollisionShape *meshCollisionShape;
        meshCollisionShape = malloc(sizeof(MeshCollisionShape));
        POINTER_CHECK(meshCollisionShape);
        meshCollisionShape->facesVertex = model->data->objects[0].facesVertex;
        meshCollisionShape->numFaces = model->data->objects[0].length;


        this->object = meshCollisionShape;
        SUPER(initialize_node);
    }

    /**
     * @brief Retrieves the priority value.
     *
     * This function assigns the priority value to the provided pointer.
     *
     * @param priority A pointer to an integer where the priority value will be stored.
     */
    void get_priority(int *priority) {
        *priority = 4;
    }

    /**
     * @brief Loads data from a file.
     *
     * This function reads data from the provided file pointer and loads it into the appropriate structures.
     *
     * @param file A pointer to the FILE object that represents the file to be read.
     */
    void load(FILE *file) {
        char path[100];
        if (file) {
            fscanf(file,"(%100[^)])", path);
        }
        this::constructor(path);
    }

    /**
     * @brief Saves the current state of the mesh_cshape object to a file.
     *
     * This function writes the necessary data of the mesh_cshape object to the provided file.
     * The file should be opened in a mode that allows writing before calling this function.
     *
     * @param file A pointer to a FILE object that identifies the file to which the data will be written.
     */
    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }




    
}

