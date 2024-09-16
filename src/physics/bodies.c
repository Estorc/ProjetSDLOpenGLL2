#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
#include "../types.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "../node.h"
#include "../window.h"
#include "physics.h"
#include "bodies.h"

/**
 * Add a child to a node
 *
 * @param node {Node*} - The parent node.
 * @param child {Node*} - The child node.
 */

void add_shape(Node *node, Node *child) {
    u8 *length;
    Node ***shapes;
    GET_FROM_BODY_NODE(node, length, length);
    GET_FROM_BODY_NODE(node, collisionsShapes, shapes);
    (*shapes)[(*length)++] = child;
    child->parent = node;
    
}

/**
 * Add a child to a node and realloc the parent
 *
 * @param node {Node*} - The parent node.
 * @param child {Node*} - The child node.
 */

void add_shape_and_realloc(Node *node, Node *child) {
    u8 *length;
    Node ***shapes;
    GET_FROM_BODY_NODE(node, length, length);
    GET_FROM_BODY_NODE(node, collisionsShapes, shapes);
    (*shapes) = realloc((*shapes), sizeof(Node *) * ((*length)+1));
    POINTER_CHECK((*shapes));
    (*shapes)[(*length)++] = child;
    child->parent = node;
}

/**
 * Remove a child from the parent
 *
 * @param node {Node*} - The parent node.
 * @param child {Node*} - The child node.
 */

void remove_shape(Node *node, Node *child) {
    u8 *length;
    Node ***shapes;
    GET_FROM_BODY_NODE(node, length, length);
    GET_FROM_BODY_NODE(node, collisionsShapes, shapes);
    for (Node **rightCursor, **leftCursor = rightCursor = (*shapes); rightCursor < (*shapes) + (*length); leftCursor++, rightCursor++) {
        if (child == *rightCursor) rightCursor++;
        if (leftCursor != rightCursor && rightCursor < (*shapes) + (*length)) *leftCursor = *rightCursor;
    }
}

/**
 * Remove a child from the parent and realloc the parent
 *
 * @param node {Node*} - The parent node.
 * @param child {Node*} - The children node.
 */

void remove_shape_and_realloc(Node *node, Node *child) {
    u8 *length;
    Node ***shapes;
    GET_FROM_BODY_NODE(node, length, length);
    GET_FROM_BODY_NODE(node, collisionsShapes, shapes);
    for (Node **rightCursor, **leftCursor = rightCursor = (*shapes); rightCursor < (*shapes) + (*length); leftCursor++, rightCursor++) {
        if (child == *rightCursor) rightCursor++;
        if (leftCursor != rightCursor && rightCursor < (*shapes) + (*length)) *leftCursor = *rightCursor;
    }
    (*shapes) = realloc((*shapes), sizeof(Node *) * (--(*length)));
    //POINTER_CHECK((*shapes));
}

/**
 * Remove a child from the parent and free the child
 *
 * @param node {Node*} - The parent node.
 * @param child {Node*} - The children node.
 */

void remove_shape_and_free(Node *node, Node *child) {
    u8 *length;
    Node ***shapes;
    GET_FROM_BODY_NODE(node, length, length);
    GET_FROM_BODY_NODE(node, collisionsShapes, shapes);
    for (Node **rightCursor, **leftCursor = rightCursor = (*shapes); rightCursor < (*shapes) + (*length); leftCursor++, rightCursor++) {
        if (child == *rightCursor) free_node(*rightCursor), rightCursor++;
        if (leftCursor != rightCursor && rightCursor < (*shapes) + (*length)) *leftCursor = *rightCursor;
    }
}

/**
 * Remove a child from the parent, realloc the parent and free the child
 *
 * @param node {Node*} - The parent node.
 * @param child {Node*} - The children node.
 */

void remove_shape_and_free_and_realloc(Node *node, Node *child) {
    u8 *length;
    Node ***shapes;
    GET_FROM_BODY_NODE(node, length, length);
    GET_FROM_BODY_NODE(node, collisionsShapes, shapes);
    for (Node **rightCursor, **leftCursor = rightCursor = (*shapes); rightCursor < (*shapes) + (*length); leftCursor++, rightCursor++) {
        if (child == *rightCursor) {
            free_node(*rightCursor);
            rightCursor++;
        }
        if (leftCursor != rightCursor && rightCursor < (*shapes) + (*length)) *leftCursor = *rightCursor;
    }
    (*shapes) = realloc((*shapes), sizeof(Node *) * (--(*length)));
    //POINTER_CHECK((*shapes));
}