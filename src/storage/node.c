#include "../types.h"
#include "../math/math_util.h"
#include "../io/model.h"
#include "../render/framebuffer.h"
#include "node.h"
#include "../render/render.h"
#include "../render/lighting.h"
#include "../classes/classes.h"
#include "../io/scene_loader.h"
#include "../scripts/scripts.h"
#include "../render/camera.h"
#include "../storage/queue.h"
#include <stdio.h>
#include <stdarg.h>



void change_scene() {

    Node **root = (Node **) queue_pop(&callQueue);
    char *path = (char *) queue_pop(&callQueue);
    Camera **camera = (Camera **) queue_pop(&callQueue);
    Script *scripts = (Script *) queue_pop(&callQueue);
    free_node(*root);
    (*root) = load_scene(path, camera, scripts);
    printf("Scene changed to %s\n", path);
    printf("Root: %p\n", *root);
}


int index_of_child(Node *node, Node *child) {
    for (int i = 0; i < node->length; i++) {
        if (child == node->children[i]) return i;
    }
    return -1;
}

void add_child(Node *node, Node *child) {
    
    node->children[node->length++] = child;
    child->parent = node;
    
}

void add_child_and_realloc(Node *node, Node *child) {
    node->children = realloc(node->children, sizeof(Node *) * (node->length+1));
    POINTER_CHECK(node->children);
    node->children[node->length++] = child;
    child->parent = node;
}

void remove_child(Node *node, Node *child) {
    for (Node **rightCursor, **leftCursor = rightCursor = node->children; rightCursor < node->children + node->length; leftCursor++, rightCursor++) {
        if (child == *rightCursor) rightCursor++;
        if (leftCursor != rightCursor && rightCursor < node->children + node->length) *leftCursor = *rightCursor;
    }
}

void remove_child_and_realloc(Node *node, Node *child) {
    for (Node **rightCursor, **leftCursor = rightCursor = node->children; rightCursor < node->children + node->length; leftCursor++, rightCursor++) {
        if (child == *rightCursor) rightCursor++;
        if (leftCursor != rightCursor && rightCursor < node->children + node->length) *leftCursor = *rightCursor;
    }
    node->children = realloc(node->children, sizeof(Node *) * (--node->length));
    //POINTER_CHECK(node->children);
}

void remove_child_and_free(Node *node, Node *child) {
    for (Node **rightCursor, **leftCursor = rightCursor = node->children; rightCursor < node->children + node->length; leftCursor++, rightCursor++) {
        if (child == *rightCursor) free_node(*rightCursor), rightCursor++;
        if (leftCursor != rightCursor && rightCursor < node->children + node->length) *leftCursor = *rightCursor;
    }
}


void remove_child_and_free_and_realloc(Node *node, Node *child) {
    for (Node **rightCursor, **leftCursor = rightCursor = node->children; rightCursor < node->children + node->length; leftCursor++, rightCursor++) {
        if (child == *rightCursor) {
            free_node(*rightCursor);
            rightCursor++;
        }
        if (leftCursor != rightCursor && rightCursor < node->children + node->length) *leftCursor = *rightCursor;
    }
    node->children = realloc(node->children, sizeof(Node *) * (--node->length));
    //POINTER_CHECK(node->children);
}


void render_node(Node *node, mat4 modelMatrix, Shader activeShader, WorldShaders *shaders) {
    
    set_shader_int(activeShader, "diffuseMapActive", 0);
    set_shader_int(activeShader, "normalMapActive", 0);
    set_shader_int(activeShader, "parallaxMapActive", 0);
    
    glm_translate(modelMatrix, (vec3){node->pos[0], node->pos[1], node->pos[2]});
    glm_rotate(modelMatrix, to_radians(node->rot[0]), (vec3){1.0f, 0.0f, 0.0f});
    glm_rotate(modelMatrix, to_radians(node->rot[1]), (vec3){0.0f, 1.0f, 0.0f});
    glm_rotate(modelMatrix, to_radians(node->rot[2]), (vec3){0.0f, 0.0f, 1.0f});
    glm_scale(modelMatrix, (vec3){node->scale[0], node->scale[1], node->scale[2]});

    METHOD(node, render, modelMatrix, activeShader, shaders);
}


void free_node(Node *node) {
    METHOD(node, free);
    printf("Free nodes!\n");
}


void print_node(Node *node, int level) {
    #ifdef DEBUG
    for (int i = 0; i < level; i++) printf("\t");
    printf(" - [%s] (%s) %c%c%c%c%c %d children:\n", 
    
            classManager.class_names[node->type],
            "NodeName",

            (!level) ? 'r' : '_', 
            (node->flags & NODE_ACTIVE) ? 'a' : '_', 
            (node->flags & NODE_VISIBLE) ? 'v' : '_', 
            (node->flags & NODE_SCRIPT) ? 's' : '_', 
            (node->shader) ? 'S' : '_', 
            
            node->length);
    for (int i = 0; i < node->length; i++) {
        print_node(node->children[i], level+1);
    }
    #endif
}