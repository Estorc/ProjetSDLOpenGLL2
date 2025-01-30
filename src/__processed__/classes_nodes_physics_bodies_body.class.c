#include <stdarg.h>
#include "../raptiquax.h"
#include "../classes/classes.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../storage/node.h"
#include "../io/scene_loader.h"
#include "../render/lighting.h"
#include "../buffer.h"
static unsigned __type__ __attribute__((unused)) = CLASS_TYPE_BODY;
#line 13 "src/classes/nodes/physics/bodies/body.class.c"
void __class_method_body_is_body(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);bool * body = va_arg(args, bool *);(void)this;
        (*body) = true;
}

#line 17 "src/classes/nodes/physics/bodies/body.class.c"
void __class_method_body_apply_impulse(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);(void)this;
        // Do nothing
}

    /**
     * @brief Adds a child shape to the specified node.
     * 
     * @param child The child shape to be added to the node.
     */

#line 27 "src/classes/nodes/physics/bodies/body.class.c"
void __class_method_body_add_shape(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Node * child = va_arg(args, Node *);(void)this;
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

#line 43 "src/classes/nodes/physics/bodies/body.class.c"
void __class_method_body_add_shape_and_realloc(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Node * child = va_arg(args, Node *);(void)this;
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

#line 60 "src/classes/nodes/physics/bodies/body.class.c"
void __class_method_body_remove_shape(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Node * child = va_arg(args, Node *);(void)this;
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

#line 77 "src/classes/nodes/physics/bodies/body.class.c"
void __class_method_body_remove_shape_and_realloc(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Node * child = va_arg(args, Node *);(void)this;
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

#line 96 "src/classes/nodes/physics/bodies/body.class.c"
void __class_method_body_remove_shape_and_free(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Node * child = va_arg(args, Node *);(void)this;
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

#line 113 "src/classes/nodes/physics/bodies/body.class.c"
void __class_method_body_remove_shape_and_free_and_realloc(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);Node * child = va_arg(args, Node *);(void)this;
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

#line 128 "src/classes/nodes/physics/bodies/body.class.c"
void __class_method_body_get_collision_normal(void * __retValueVP__, va_list args) {Node * this = va_arg(args, Node *);float * normal = va_arg(args, float *);(void)this;
        glm_vec3_zero(normal);
}
