/**
 * @file
 * @brief This file defines the body class for the physics engine.
 *
 * This file contains the implementation of the body class, which is used to represent physical bodies
 * in the physics engine. It includes functions for initializing, updating, and managing the state of
 * physical bodies within the simulation.
 *
 * It is used for handling the physical properties and behaviors of objects in the simulation, such as
 * position, velocity, and collision detection.
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
#include "io/scene_loader.h"
#include "render/lighting.h"
#include "buffer.h"

/**
 * @ingroup Classes Classes
 * @{
 */
class Body : public PhysicalNode {
    __containerType__ Node *
    public:

    /**
     * @brief Checks if the given pointer represents a body.
     *
     * This function takes a pointer to a boolean and performs a check to determine if it represents a body.
     *
     * @param body A pointer to a boolean that will be checked.
     */
    void is_body(bool *body) {
        (*body) = true;
    }

    /**
     * @brief Applies an impulse to the physics body.
     *
     * This function is responsible for applying an impulse to the physics body,
     * which will affect its velocity and position based on the physics simulation.
     *
     * @note The specific details of how the impulse is calculated and applied
     *       are not provided in this snippet.
     */
    void apply_impulse() {
        // Do nothing
    }

    /**
     * @brief Adds a child shape to the specified node.
     * 
     * @param child The child shape to be added to the node.
     */
    void add_shape(Node *child) {
        u8 *length;
        Node ***shapes;
        this::get_collisions_shapes(&shapes, &length);
        (*shapes)[(*length)++] = child;
        child->parent = this;
        
    }

    /**
     * @brief Adds a child shape to the specified node and reallocates memory if necessary.
     * 
     * @param node The parent node to which the child shape will be added.
     * @param child The child shape to be added to the node.
     */
    void add_shape_and_realloc(Node *child) {
        u8 *length;
        Node ***shapes;
        this::get_collisions_shapes(&shapes, &length);
        (*shapes) = realloc((*shapes), sizeof(Node *) * ((*length)+1));
        POINTER_CHECK((*shapes));
        (*shapes)[(*length)++] = child;
        child->parent = this;
    }

    /**
     * @brief Removes a child shape from the specified node.
     * 
     * @param node The parent node from which the child shape will be removed.
     * @param child The child shape to be removed from the node.
     */
    void remove_shape(Node *child) {
        u8 *length;
        Node ***shapes;
        this::get_collisions_shapes(&shapes, &length);
        for (Node **rightCursor, **leftCursor = rightCursor = (*shapes); rightCursor < (*shapes) + (*length); leftCursor++, rightCursor++) {
            if (child == *rightCursor) rightCursor++;
            if (leftCursor != rightCursor && rightCursor < (*shapes) + (*length)) *leftCursor = *rightCursor;
        }
    }

    /**
     * @brief Removes a child shape from the specified node and reallocates memory if necessary.
     * 
     * @param node The parent node from which the child shape will be removed.
     * @param child The child shape to be removed from the node.
     */
    void remove_shape_and_realloc(Node *child) {
        u8 *length;
        Node ***shapes;
        this::get_collisions_shapes(&shapes, &length);
        for (Node **rightCursor, **leftCursor = rightCursor = (*shapes); rightCursor < (*shapes) + (*length); leftCursor++, rightCursor++) {
            if (child == *rightCursor) rightCursor++;
            if (leftCursor != rightCursor && rightCursor < (*shapes) + (*length)) *leftCursor = *rightCursor;
        }
        (*shapes) = realloc((*shapes), sizeof(Node *) * (--(*length)));
        //POINTER_CHECK((*shapes));
    }

    /**
     * @brief Removes a child shape from the specified node and frees the memory allocated for the child shape.
     * 
     * @param node The parent node from which the child shape will be removed.
     * @param child The child shape to be removed and freed.
     */
    void remove_shape_and_free(Node *child) {
        u8 *length;
        Node ***shapes;
        this::get_collisions_shapes(&shapes, &length);
        for (Node **rightCursor, **leftCursor = rightCursor = (*shapes); rightCursor < (*shapes) + (*length); leftCursor++, rightCursor++) {
            if (child == *rightCursor) (*rightCursor)::free(), rightCursor++;
            if (leftCursor != rightCursor && rightCursor < (*shapes) + (*length)) *leftCursor = *rightCursor;
        }
    }

    /**
     * @brief Removes a child shape from the specified node, frees the memory allocated for the child shape, and reallocates memory if necessary.
     * 
     * @param node The parent node from which the child shape will be removed.
     * @param child The child shape to be removed, freed, and reallocated.
     */
    void remove_shape_and_free_and_realloc(Node *child) {
        u8 *length;
        Node ***shapes;
        this::get_collisions_shapes(&shapes, &length);
        for (Node **rightCursor, **leftCursor = rightCursor = (*shapes); rightCursor < (*shapes) + (*length); leftCursor++, rightCursor++) {
            if (child == *rightCursor) {
                (*rightCursor)::free();
                rightCursor++;
            }
            if (leftCursor != rightCursor && rightCursor < (*shapes) + (*length)) *leftCursor = *rightCursor;
        }
        (*shapes) = realloc((*shapes), sizeof(Node *) * (--(*length)));
        //POINTER_CHECK((*shapes));
    }

    /**
     * @brief Calculates the collision normal vector.
     *
     * This function computes the normal vector of a collision and stores it in the provided array.
     *
     * @param[out] normal A pointer to an array of floats where the collision normal vector will be stored.
     */
    void get_collision_normal(float *normal) {
        glm_vec3_zero(normal);
    }
}

