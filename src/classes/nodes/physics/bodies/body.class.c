#include "math/math_util.h"
#include "io/model.h"
#include "render/framebuffer.h"
#include "storage/node.h"
#include "io/scene_loader.h"
#include "render/lighting.h"
#include "buffer.h"

class Body : public PhysicalNode {
    __containerType__ Node *
    public:

    void is_body(bool *body) {
        (*body) = true;
    }

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

    void get_collision_normal(float *normal) {
        glm_vec3_zero(normal);
    }
}