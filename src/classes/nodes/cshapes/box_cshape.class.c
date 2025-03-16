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
class BoxCShape : public CShape {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the BoxCShape class.
     *
     * This function initializes a new instance of the BoxCShape class. It sets up the
     * necessary properties and state for the box shape.
     */
    void constructor() {
        this->type = __type__;

        BoxCollisionShape *boxCollisionShape;
        boxCollisionShape = malloc(sizeof(BoxCollisionShape));
        POINTER_CHECK(boxCollisionShape);

        this->object = boxCollisionShape;
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
        *priority = 0;
    }

    /**
     * @brief Loads the box collision shape.
     *
     * This function is responsible for initializing and loading the necessary
     * resources for the box collision shape. It sets up the vertices, indices,
     * and other required data for rendering the box shape.
     */
    void load() {
        this::constructor();
    }

    /**
     * @brief Saves the current state of the BoxCShape to a file.
     *
     * This function writes the current state of the BoxCShape object to the specified file.
     * The file should be opened in a mode that allows writing.
     *
     * @param file A pointer to the FILE object where the state will be saved.
     */
    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }


    /**
     * @brief Static instance of the Model structure.
     *
     * This static variable holds the model data for the box shape. It is 
     * initialized to zero and used throughout the BoxCShape class to represent 
     * the box shape.
     */
    static Model _model = {0};

    /**
     * @brief Retrieves the model associated with the BoxCShape.
     *
     * This function assigns the model associated with the BoxCShape to the provided
     * pointer. The model is represented by the Model structure.
     *
     * @param[out] model A double pointer to the Model structure where the model
     *                   will be stored.
     */
    void get_model(Model **model) {
        if (!_model.data) {
            load_model("assets/models/collisionShapes/box.obj", &_model.data);
        }
        *model = &_model;
    }

    
}


