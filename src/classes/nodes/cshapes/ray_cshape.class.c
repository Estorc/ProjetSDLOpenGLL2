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

/**
 * @ingroup Classes Classes
 * @{
 */
class RayCShape : public CShape {
    __containerType__ Node *
    public:

    /**
     * @brief Constructor for the ray_cshape class.
     *
     * This function initializes a new instance of the ray_cshape class.
     * It sets up the necessary properties and prepares the object for use.
     */
    void constructor() {
        this->type = __type__;

        RayCollisionShape *rayCollisionShape;
        rayCollisionShape = malloc(sizeof(RayCollisionShape));
        POINTER_CHECK(rayCollisionShape);

        this->object = rayCollisionShape;
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
        *priority = 5;
    }

    /**
     * @brief Loads the ray_cshape class.
     *
     * This function is responsible for initializing and loading the necessary
     * resources for the ray_cshape class. It sets up the required data structures
     * and prepares the class for use.
     */
    void load() {
        this::constructor();
    }

    /**
     * @brief Saves the current state to the specified file.
     *
     * This function writes the current state of the object to the provided file.
     *
     * @param file A pointer to the FILE object where the state will be saved.
     */
    void save(FILE *file) {
        fprintf(file, "%s", classManager.class_names[this->type]);
    }
}

