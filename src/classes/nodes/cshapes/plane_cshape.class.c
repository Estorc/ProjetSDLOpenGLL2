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
class PlaneCShape : public CShape {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the PlaneCShape class.
     *
     * This function initializes a new instance of the PlaneCShape class. It sets up
     * the necessary properties and resources required for the plane shape.
     */
    void constructor() {
        this->type = __type__;

        PlaneCollisionShape *planeCollisionShape;
        planeCollisionShape = malloc(sizeof(PlaneCollisionShape));
        POINTER_CHECK(planeCollisionShape);

        this->object = planeCollisionShape;
        SUPER(initialize_node);
    }

    /**
     * @brief Retrieves the priority value.
     *
     * This function assigns the priority value to the provided pointer.
     *
     * @param[out] priority Pointer to an integer where the priority value will be stored.
     */
    void get_priority(int *priority) {
        *priority = 2;
    }

    /**
     * @brief Loads the plane shape data.
     *
     * This function is responsible for loading the necessary data for the plane shape.
     * It initializes the required resources and prepares the plane shape for rendering.
     */
    void load() {
        this::constructor();
    }

    /**
     * @brief Saves the current state of the plane shape to a file.
     *
     * This function writes the current state of the plane shape to the specified file.
     *
     * @param file A pointer to the FILE object where the plane shape state will be saved.
     */
    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }

    /**
     * @brief Static model data for the plane shape.
     *
     * This static variable holds the model data for the plane shape. It is initialized
     * to zero and is used to store the model's properties and attributes.
     */
    static Model _model = {0};

    /**
     * @brief Retrieves the model.
     *
     * This function assigns a pointer to a Model structure to the provided
     * pointer-to-pointer parameter. The function is expected to populate the
     * provided pointer with a valid Model instance.
     *
     * @param model A double pointer to a Model structure. The function will
     *              assign a pointer to a Model instance to this parameter.
     */
    void get_model(Model **model) {
        if (!_model.data) {
            load_model("assets/models/collisionShapes/plane.obj", &_model.data);
        }
        *model = &_model;
    }
    
}

