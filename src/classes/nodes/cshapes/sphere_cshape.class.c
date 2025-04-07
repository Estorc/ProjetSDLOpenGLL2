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

#include <raptiquax.h>
#include <classes/classes.h>
#include <math/math_util.h>
#include <io/model.h>
#include <render/framebuffer.h>
#include <storage/node.h>

/**
 * @ingroup Classes Classes
 * @{
 */
class SphereCShape : public CShape {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the SphereCShape class.
     *
     * This function initializes a new instance of the SphereCShape class.
     * It sets up the necessary properties and resources for the sphere shape.
     */
    void constructor() {
        this->type = __type__;

        SphereCollisionShape *sphereCollisionShape;
        sphereCollisionShape = malloc(sizeof(SphereCollisionShape));
        POINTER_CHECK(sphereCollisionShape);

        this->object = sphereCollisionShape;
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
        *priority = 1;
    }

    /**
     * @brief Loads the sphere shape data.
     *
     * This function is responsible for loading the necessary data to represent
     * a sphere shape in the application. It initializes the required resources
     * and prepares the sphere shape for rendering.
     */
    void load() {
        this::constructor();
    }

    /**
     * @brief Saves the current state of the sphere shape to a file.
     *
     * This function writes the current state of the sphere shape to the specified file.
     *
     * @param file A pointer to the FILE object where the sphere shape state will be saved.
     */
    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }

    /**
     * @brief Static variable to store the model data for the sphere.
     *
     * The _model variable is a static instance of the Model structure. It is
     * initialized to zero and is used to store the model data for the sphere
     * shape. Being static, it is only accessible within this file.
     */
    static Model _model = {0};
    
    /**
     * @brief Retrieves the model.
     *
     * This function retrieves the model and assigns it to the provided pointer.
     *
     * @param model A double pointer to a Model structure where the retrieved model will be stored.
     */
    void get_model(Model **model) {
        if (!_model.data) {
            load_model("assets/models/collisionShapes/sphere.obj", &_model.data);
        }
        *model = &_model;
    }


    
}

