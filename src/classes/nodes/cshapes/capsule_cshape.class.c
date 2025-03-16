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

/**
 * @ingroup Classes Classes
 * @{
 */
class CapsuleCShape : public CShape {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the capsule shape class.
     *
     * This function initializes a new instance of the capsule shape class.
     * It sets up the necessary properties and prepares the object for use.
     */
    void constructor() {
        this->type = __type__;

        CapsuleCollisionShape *capsuleCollisionShape;
        capsuleCollisionShape = malloc(sizeof(CapsuleCollisionShape));
        POINTER_CHECK(capsuleCollisionShape);

        this->object = capsuleCollisionShape;
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
        *priority = 3;
    }

    /**
     * @brief Loads the capsule shape data.
     *
     * This function is responsible for loading the necessary data for the capsule shape.
     * It initializes the shape properties and prepares it for rendering or other operations.
     */
    void load() {
        this::constructor();
    }

    /**
     * @brief Saves the current state to a file.
     *
     * This function writes the current state of the object to the specified file.
     *
     * @param file A pointer to the file where the state will be saved.
     */
    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }

    /**
     * @brief Static model instance for the capsule shape.
     *
     * This static variable holds the model data for the capsule shape. It is initialized
     * to zero and used throughout the application to represent the capsule shape.
     */
    static Model _model = {0};

    /**
     * @brief Retrieves the model associated with the current object.
     *
     * This function assigns the model associated with the current object to the 
     * provided pointer. The model is returned as a pointer to a pointer of type Model.
     *
     * @param model A double pointer to a Model structure where the model will be stored.
     */
    void get_model(Model **model) {
        if (!_model.data) {
            load_model("assets/models/collisionShapes/sphere.obj", &_model.data);
        }
        *model = &_model;
    }


    
}

